/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RarManager.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:55:04 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/05 18:33:14 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/RarManager.hpp"
#include "../00-headers/01-core/HttpRequest.hpp"
#include "../00-headers/01-core/HttpResponse.hpp"
#include "../00-headers/01-core/Cookies.hpp"
#include "../00-headers/01-core/SignalHandler.hpp"
#include "../00-headers/02-utils/signals.hpp"
#include "../00-headers/02-utils/Log.hpp"
#include "../00-headers/03-cgi/cgi_handler.hpp"
#include <sys/select.h>

int findServerIndex(std::string const& request, std::vector<Server *>& Servers) 
{
    std::string host;
    int port = 8080; 

    size_t hostPos = request.find("Host: ");
    if (hostPos != std::string::npos) 
    {
        hostPos += 6;  
        size_t endPos = request.find("\r\n", hostPos);
        std::string hostPort = request.substr(hostPos, endPos - hostPos);        
        size_t colonPos = hostPort.find(":");
        if (colonPos != std::string::npos) 
        {
            host = hostPort.substr(0, colonPos);
            std::istringstream iss(hostPort.substr(colonPos + 1));
            iss >> port;
        } 
        else 
            host = hostPort;
    }
    for (size_t i = 0; i < Servers.size(); ++i) 
    {
        const std::vector<int>& serverPorts = Servers[i]->getPorts(); 
        if (Servers[i]->getConf()->getConfig("host") == host && 
            std::find(serverPorts.begin(), serverPorts.end(), port) != serverPorts.end()) 
            return i;
    }
    return 0;
}

void type_request_manager(int *fd_client, std::string *req, Server *server, SessionManager &sessionManager)
{            
    HttpRequest request(*req, server);
    HttpResponse response(request);
    std::ostringstream msg_size;
    msg_size << request.getBody().size();
    msg_size << " bytes";
    Log::print_final_log("Request:", request.getMethod(), request.getURI(), msg_size.str());
    if (request.getMethod() == "PUT" && !request.isScript())
    {
        response.put(request);
        response.setStatusCode(201);
        response.send(*fd_client);
    }
    response.setResourcePath(request);
    if (request.isStatic())
    {
        std::string cookieHeader = request.getHeader("Cookie");
        Cookies cookies(cookieHeader);
        std::string sessionId = cookies.getCookie("sessionId");
        if ((!sessionManager.sessionExist(sessionId) || sessionId.empty()))
        {
            sessionId = sessionManager.createSessions();
            cookies.setCookie("sessionId", sessionId);
        }
        response.addHeader("Set-Cookie", cookies.getSetCookieHeader().substr(12));
    }
    std::string filePath = response.getFilePath();
    filePath = removeDuplicateSlashes(filePath);            
    if ((filePath.find("cgi") != std::string::npos || request.hasFileSpecialRoute(getExtension(filePath)))) 
    {   
        Location *route = NULL;
        if (request.hasFileSpecialRoute(getExtension(filePath)))
            route = server->getRoute(getExtension(filePath));
        if (filePath.find("cgi") != std::string::npos)
            route = server->getRoute("/cgi-bin/");
        if (!route)
            return;
        else if (response.isAllowedMethod(route, request))
        {
            Cgi_handler cgiHandler;
            cgiHandler.executeCGI(filePath, request, *fd_client, response);              
        }
        else
        { 
            response.setRedirection(406);
            response.send(*fd_client);
        }
    }
    else if (filePath.find("cgi-bin/") != std::string::npos && !server->getCgiStatus())
    { 
        response.setRedirection(403);
        response.send(*fd_client);
    }
    else   
        response.send(*fd_client);
    std::string connectionHeader = request.getHeader("Connection");
    req->clear();
}

void request_and_response_fd_manager(std::vector<int>& fd_sockets, std::vector<Server*>& Servers, SessionManager &sessionManager)
{
    try
    {
        std::map<int, std::string> requestMap;
        Epoll epoll(10);
        char buff[BUFFER_SIZE];
        int fd_client;
        ssize_t reads;
        struct epoll_event event;
        struct sockaddr_in client_addr;
        socklen_t client_addr_len;
        
        for (size_t i = 0; i < fd_sockets.size(); ++i)
        {
            epoll.addFd(fd_sockets[i], EPOLLIN);
            epoll.makeSocketNonBlocking(fd_sockets[i]);
        }
        epoll.addFd(signalPipeFd[0], EPOLLIN);
        while (!signalReceived)
        {
            int eventCount = epoll.wait(-1);
            if (eventCount == -1)
            {
                Log::error("Erreur lors de epoll_wait");
                break;
            }
            for (int i = 0; i < eventCount; ++i)
            {
                event = epoll.getEvent(i);

                if (event.data.fd == signalPipeFd[0]) 
                {
                    signalReceived = true;
                    break;
                }
                bool isServerSocket = false;
                for (size_t j = 0; j < fd_sockets.size(); ++j)
                {
                    if (event.data.fd == fd_sockets[j])
                    {
                        isServerSocket = true;
                        break;
                    }
                }
                if (isServerSocket) 
                {
                    client_addr_len = sizeof(client_addr);
                    fd_client = accept(event.data.fd, (struct sockaddr*)&client_addr, &client_addr_len);
                    if (fd_client == -1)
                    {
                        Log::print_final_log("Erreur lors de accept", "FD:", strerror(errno));
                        continue;
                    }
                    epoll.makeSocketNonBlocking(fd_client);
                    epoll.addFd(fd_client, EPOLLIN);
                }
                else if (event.events & EPOLLIN) 
                {
                    fd_client = event.data.fd;
                    std::string &currentRequest = requestMap[fd_client];
                    while ((reads = recv(fd_client, buff, BUFFER_SIZE, 0)) > 0)
                        currentRequest.append(buff, reads);
                    if (reads == 0) 
                    {
                        Log::print_final_log("Connection closed by client", "FD:", fd_client);
                        epoll.removeFd(fd_client);
                        ::close(fd_client);
                        requestMap.erase(fd_client);
                        continue;
                    }
                    else if (reads < 0 && errno != EAGAIN && errno != EWOULDBLOCK) 
                    {
                        Log::print_final_log("Error in recv", "FD: ", strerror(errno));
                        epoll.removeFd(fd_client);
                        ::close(fd_client);
                        requestMap.erase(fd_client);
                        continue;
                    }
                    size_t headerEnd = currentRequest.find("\r\n\r\n");
                    if (headerEnd != std::string::npos)
                    {
                        if (currentRequest.find("Transfer-Encoding: chunked") != std::string::npos)
                        {
                            size_t chunk_start = headerEnd + 4;
                            while (true)
                            {
                                size_t chunk_size_end = currentRequest.find("\r\n", chunk_start);
                                if (chunk_size_end == std::string::npos)
                                    break;
                                std::string chunk_size_str = currentRequest.substr(chunk_start, chunk_size_end - chunk_start);
                                size_t chunk_size = std::strtol(chunk_size_str.c_str(), NULL, 16);
                                if (chunk_size == 0)
                                {
                                    Log::print_final_log("Chunked request received:", "FD:", fd_client);
                                    int serverIndex = findServerIndex(currentRequest, Servers);
                                    type_request_manager(&fd_client, &currentRequest, Servers[serverIndex], sessionManager);
                                    currentRequest.clear();
                                    break;
                                }
                                size_t chunk_data_start = chunk_size_end + 2;
                                size_t chunk_data_end = chunk_data_start + chunk_size;

                                if (chunk_data_end > currentRequest.size())
                                    break;
                                chunk_start = chunk_data_end + 2; 
                            }
                        }
                        else
                        {
                            size_t content_length_pos = currentRequest.find("Content-Length:");
                            if (content_length_pos != std::string::npos)
                            {
                                size_t content_length_start = content_length_pos + strlen("Content-Length:");
                                size_t content_length_end = currentRequest.find("\r\n", content_length_start);
                                if (content_length_end != std::string::npos)
                                {
                                    std::string content_length_str = currentRequest.substr(content_length_start, content_length_end - content_length_start);
                                    size_t content_length = std::atoi(content_length_str.c_str());
                                    size_t body_start = headerEnd + 4;

                                    if (currentRequest.size() >= body_start + content_length)
                                    {
                                        Log::print_final_log("Content-Length request received:", "FD:", fd_client);
                                        int serverIndex = findServerIndex(currentRequest, Servers);
                                        type_request_manager(&fd_client, &currentRequest, Servers[serverIndex], sessionManager);
                                        currentRequest.clear();
                                    }
                                    else
                                        Log::print_final_log("Incomplete content-length data detected:", "FD:", fd_client);
                                }
                            }
                            else
                            {
                                Log::print_final_log("No Content-Length or chunked data detected:", "FD:", fd_client);
                                int serverIndex = findServerIndex(currentRequest, Servers);
                                type_request_manager(&fd_client, &currentRequest, Servers[serverIndex], sessionManager);
                                currentRequest.clear();
                            }
                        }
                    }
                }
            }
        }
        if (signalReceived)
        {
            sessionManager.saveSessionsToFile();
            Log::cleanup();
            throw SignalException();
        }
    }
    catch (std::exception& e)
    {
        Log::print_final_log("Closing:", e.what());
        Log::print_final_log("Server powered off", "Goodbye!");
    }
}
