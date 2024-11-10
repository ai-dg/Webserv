#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include "../headers/RarManager.hpp"
#include "../headers/includes.hpp"
#include "../headers/HttpRequest.hpp"
#include "../headers/HttpResponse.hpp"
#include "../headers/cgi_handler.hpp"
#include "../headers/Cookies.hpp"
#include "../headers/signals.hpp"
#include "../headers/Log.hpp"

volatile sig_atomic_t sig_g = 0;

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
        {
            host = hostPort;
        }
    }

    Log::output("./sessions/find_server_index.txt") << "Request Host: " << host << ", Port: " << port << std::endl;

    
    for (size_t i = 0; i < Servers.size(); ++i) 
    {
        Log::output("./sessions/find_server_index.txt") << "Checking Server index " << i << std::endl;
        Log::output("./sessions/find_server_index.txt") << "Server Host: " << Servers[i]->getConf()->getConfig("host") << ", Ports: ";
        
        
        const std::vector<int>& serverPorts = Servers[i]->getPorts();
        for (std::vector<int>::const_iterator it = serverPorts.begin(); it != serverPorts.end(); ++it)
            Log::output("./sessions/find_server_index.txt") << *it << " ";
        Log::output("./sessions/find_server_index.txt") << std::endl;
        
        
        if (Servers[i]->getConf()->getConfig("host") == host && 
            std::find(serverPorts.begin(), serverPorts.end(), port) != serverPorts.end()) 
        {
            Log::output("./sessions/find_server_index.txt") << "Match found at index " << i << std::endl;
            return i; 
        }
    }

    
    Log::output("./sessions/find_server_index.txt") << "No match found, defaulting to index 0" << std::endl;
    Log::output("./sessions/find_server_index.txt") << Servers[0]->getConf()->getConfig("host") << std::endl;
    return 0;
}

void type_request_manager(int *fd_client, std::string *req, Server *server, Epoll *epoll, SessionManager &sessionManager)
{
    Log::output("./sessions/fd_client2.txt") << "**************************" << std::endl;
    Log::output("./sessions/fd_client2.txt") << "Requête complète : " << std::endl;
    Log::output("./sessions/fd_client2.txt") << *req << std::endl;
    Log::output("./sessions/fd_client2.txt") << "*************************************" << std::endl;
    
    size_t headerEnd = req->find("\r\n\r\n");
    if (headerEnd != std::string::npos) 
    {
        HttpRequest request(*req, server);
        std::string cookieHeader = request.getHeader("Cookie");
        Cookies cookies(cookieHeader);
        std::string sessionId = cookies.getCookie("sessionId");

        if (!sessionManager.sessionExist(sessionId) || sessionId.empty())
        {
            sessionId = sessionManager.createSessions();
            cookies.setCookie("sessionId", sessionId);
        }

        HttpResponse response(request);
        response.setResourcePath(request);
        response.addHeader("Set-Cookie", cookies.getSetCookieHeader().substr(12));
        
        std::string filePath = response.getFilePath();
        std::cerr << "before cgi : " << filePath <<  "   - cgi status " << server->getCgiStatus();
        if (filePath.find("cgi-bin/") != std::string::npos && server->getCgiStatus()) 
        {
            // if (request.isValidBodySize())
            // {
                Cgi_handler cgiHandler;
                std::cerr << "  - 1 " << std::endl;
                cgiHandler.executeCGI(filePath, request, *fd_client);
            // }
            // else 
            // {
            //     response.setRedirection(413);
            //     response.send(*fd_client);
            // }
        }
        else if (filePath.find("cgi-bin/") != std::string::npos && !server->getCgiStatus())
        {
            std::cerr << "  - 2 " << std::endl;
            response.setRedirection(403);
            response.send(*fd_client);
        }
        else 
        {
            std::cerr << "  - 3 " << std::endl;
            response.send(*fd_client);
        }

        std::string connectionHeader = request.getHeader("Connection");
        if (connectionHeader != "keep-alive") 
        {
            epoll->removeFd(*fd_client);
            close(*fd_client);
        }
        sessionManager.saveSessionsToFile();
        req->clear();
    }
    else
    {
        Log::output("./logs/error.log") << "Requête incomplète : en attente de plus de données." << std::endl;
    }
}

void request_and_response_fd_manager(std::vector<int>& fd_sockets, std::vector<Server*>& Servers, SessionManager &sessionManager)
{
    std::map<int, std::string> requestMap;
    Epoll epoll(10);
    char buff[BUFFER_SIZE];
    int fd_client;
    int reads;
    struct epoll_event event;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;

    for (size_t i = 0; i < fd_sockets.size(); ++i)
    {
        epoll.addFd(fd_sockets[i], EPOLLIN);  
        epoll.makeSocketNonBlocking(fd_sockets[i]);  
    }

    while (sig_g != SIGINT) 
    {
        int eventCount = epoll.wait(-1);
        for (int i = 0; i < eventCount; ++i) 
        {
            event = epoll.getEvent(i);
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
                    Log::error("accept");
                    continue;
                }
                epoll.makeSocketNonBlocking(fd_client);
                epoll.addFd(fd_client, EPOLLIN | EPOLLET);
            }
            else if (event.events & EPOLLIN) 
            {
                fd_client = event.data.fd;
                while ((reads = read(fd_client, buff, BUFFER_SIZE)) > 0) 
                {
                    requestMap[fd_client] += std::string(buff, reads);
                    bzero(buff, BUFFER_SIZE);
                }
                std::string& req = requestMap[fd_client];
                size_t headerEndPos = req.find("\r\n\r\n");
                ssize_t contentLength = -1;

                if (headerEndPos != std::string::npos) 
                {
                    size_t contentLengthPos = req.find("Content-Length: ");
                    if (contentLengthPos != std::string::npos) 
                    {
                        contentLengthPos += 16;
                        size_t endPos = req.find("\r\n", contentLengthPos);
                        std::string contentLengthStr = req.substr(contentLengthPos, endPos - contentLengthPos);
                        std::istringstream iss(contentLengthStr);
                        iss >> contentLength;
                    }
                    ssize_t totalRead = req.size();
                    
                    if (contentLength == -1 || totalRead >= (ssize_t)headerEndPos + 4 + contentLength) 
                    {
                        int serverIndex = findServerIndex(req, Servers);                        
                        type_request_manager(&fd_client, &req, Servers[serverIndex], &epoll, sessionManager);                        
                        requestMap.erase(fd_client);
                    }
                }
                if (reads == 0) 
                {
                    epoll.removeFd(fd_client);
                    close(fd_client);
                    requestMap.erase(fd_client);
                }
            }
        }
    }
}
