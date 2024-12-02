/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RarManager.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:55:04 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/02 17:16:37 by dagudelo         ###   ########.fr       */
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

std::string decode_chunked_body(const std::string& chunkedBody) {
    std::istringstream stream(chunkedBody);
    std::string decodedBody;
    std::string line;

    while (std::getline(stream, line)) {
        // Ignorer les lignes vides ou uniquement avec \r
        if (line.empty() || line == "\r") {
            continue;
        }

        // Convertir la taille du chunk de hexadécimal à entier
        size_t chunkSize = 0;
        std::stringstream chunkSizeStream(line);
        chunkSizeStream >> std::hex >> chunkSize;

        if (chunkSize == 0) {
            break; // Fin des chunks
        }

        // Lire le chunk en fonction de sa taille
        char* buffer = new char[chunkSize];
        stream.read(buffer, chunkSize);
        decodedBody.append(buffer, chunkSize);
        delete[] buffer;

        // Ignorer le \r après chaque chunk
        stream.get();
    }

    return decodedBody;
}


void send_valid_body(int fd_client, const std::string& body) {   
    std::ostringstream oss;
    oss << "Status: 200 OK\r\n";
    oss << "Content-Type: text/html; charset=utf-8\r\n";
    oss << "Content-Length: " << body.size() << "\r\n\r\n";
    oss << body;

    std::string res = oss.str();
    write(fd_client, res.c_str(), res.size());

    // Sauvegarder la réponse pour inspection
    std::ofstream file("./sessions/fd_client.txt");
    if (file.is_open()) {
        file << res;
        file.close();
    } else {
        Log::output("./logs/error.log") << "Erreur : impossible d'ouvrir le fichier ./sessions/fd_client.txt" << std::endl;
    }
}




void type_request_manager(int *fd_client, std::string *req, Server *server, Epoll *epoll, SessionManager &sessionManager)
{
    Log::output("./sessions/fd_client2.txt") << "**************************" << std::endl;
    Log::output("./sessions/fd_client2.txt") << "Requête complète : " << std::endl;
    Log::output("./sessions/fd_client2.txt") << *req << std::endl;
    Log::output("./sessions/fd_client2.txt") << "*************************************" << std::endl;
    // static int count = 1;

    size_t headerEnd = req->find("\r\n\r\n");
    if (headerEnd != std::string::npos) 
    {
             
           
        HttpRequest request(*req, server);
        HttpResponse response(request);
        //////////////////////// A TESTER !!!!!!!
        if (request.getMethod() == "PUT" && !request.isScript())
        {
            response.put(request);
            response.setStatusCode(201);
            std::cout << RED << "#1 send" << RESET << std::endl;
            response.send(*fd_client);
        }
        
        response.setResourcePath(request);
        if (!request.isStatic())
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
        // verifier le status de la methode de la route...
        if ((filePath.find("cgi") != std::string::npos || request.hasFileSpecialRoute(getExtension(filePath))) && server->getCgiStatus(filePath)) 
        {
            Location *route = server->getRoute(getExtension(filePath));
            if (!route)
                return;
            else if (response.isAllowedMethod(route, request))
            {
                Cgi_handler cgiHandler;
                std::cout << RED << "#2 send" << RESET << std::endl;
                cgiHandler.executeCGI(filePath, request, *fd_client);
                std::cerr << "CGI executed" << std::endl;
                
            }
            else
            {
                response.setRedirection(406);
                std::cout << RED << "#3 send" << RESET << std::endl;
                response.send(*fd_client);
            }
        }
        else if (filePath.find("cgi-bin/") != std::string::npos && !server->getCgiStatus())
        {
            response.setRedirection(403);
            std::cout << RED << "#4 send" << RESET << std::endl;
            response.send(*fd_client);
        }
        else 
        {
            std::cout << RED << "#5 send" << RESET << std::endl;
            response.send(*fd_client);
        }
        std::string connectionHeader = request.getHeader("Connection");
            // std::cerr << BOLD_BLUE << "connectionHeader : " << connectionHeader << RESET << std::endl;
        /*if (connectionHeader != "keep-alive") 
        {
            epoll->removeFd(*fd_client);
            close(*fd_client);
        }*/
       (void) epoll;
        sessionManager.saveSessionsToFile();
        req->clear();
    }
    else
        Log::output("./logs/error.log") << "Requête incomplète : en attente de plus de données." << std::endl;

    // std::string outputPath = "./sessions/fd_client_final.txt";
    // const size_t bufferSize = 4096; // Taille du buffer pour les lectures
    // char buffer[bufferSize];
    // ssize_t bytesRead;

    // // Ouvrir un fichier pour écrire
    // std::ofstream outputFile(outputPath.c_str(), std::ios::out | std::ios::binary);
    // if (!outputFile.is_open()) {
    //     std::cerr << "Error: Unable to open file " << outputPath << " for writing." << std::endl;
    //     return;
    // }

    // // Lire les données depuis fd_client
    // while ((bytesRead = read(*fd_client, buffer, bufferSize)) > 0) {
    //     // Écrire les données lues dans le fichier
    //     outputFile.write(buffer, bytesRead);
    //     if (outputFile.fail()) {
    //         std::cerr << "Error: Failed to write to file " << outputPath << "." << std::endl;
    //         break;
    //     }
    // }

    // if (bytesRead == -1) {
    //     std::cerr << "Error: Failed to read from fd_client: " << strerror(errno) << std::endl;
    // }

    // outputFile.close();

    // if (bytesRead != -1) {
    //     std::cout << "Data successfully written to " << outputPath << "." << std::endl;
    // }

    // char buff_2[BUFFER_SIZE];
    // ssize_t bytesRead;
    // std::string finalBuffer;
    // while((bytesRead = read(*fd_client, buff_2, BUFFER_SIZE)) > 0)
    // {
    //     finalBuffer.append(buff_2, bytesRead);
    // }
    // if (bytesRead == -1)
    // {
    //     Log::output("./logs/error.log") << "Error reading from client socket" << std::endl;
    // }
    // Log::output("./sessions/fd_client_final.txt") << finalBuffer << std::endl;
    
}

void request_and_response_fd_manager(std::vector<int>& fd_sockets, std::vector<Server*>& Servers, SessionManager &sessionManager)
{
    try
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
        
        epoll.addFd(signalPipeFd[0], EPOLLIN);

        while (!signalReceived) 
        {
            int eventCount = epoll.wait(-1);
            if (eventCount == -1)
            {
                Log::error("Fatal error during epoll_wait");
                break;
            }

            if (eventCount == 0)
                continue;

            for (int i = 0; i < eventCount && !signalReceived; ++i) 
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
                            //close(fd_client);
                        }
                    }
                    if (reads == 0) 
                    
                    {
                        //epoll.removeFd(fd_client);
                        //close(fd_client);
                        requestMap.erase(fd_client);
                    }
                }
            }
        }
        if (signalReceived)
        {
            //sessionManager.saveSessionsToFile();
            Log::cleanup();
            throw SignalException();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
