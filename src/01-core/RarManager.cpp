/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RarManager.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:55:04 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/04 12:28:55 by dagudelo         ###   ########.fr       */
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
        
        if (line.empty() || line == "\r") {
            continue;
        }

        
        size_t chunkSize = 0;
        std::stringstream chunkSizeStream(line);
        chunkSizeStream >> std::hex >> chunkSize;

        if (chunkSize == 0) {
            break; 
        }

        
        char* buffer = new char[chunkSize];
        stream.read(buffer, chunkSize);
        decodedBody.append(buffer, chunkSize);
        delete[] buffer;

        
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

    // size_t headerEnd = req->find("\r\n\r\n");
    // if (headerEnd != std::string::npos) 
    // {
             
           
        HttpRequest request(*req, server);
        HttpResponse response(request);
        //////////////////////// A TESTER !!!!!!!
        if (request.getMethod() == "PUT" && !request.isScript())
        {
            response.put(request);
            response.setStatusCode(201);
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
        if (server->getCgiStatus(filePath))
            std::cerr << "filepath " << filePath << "   true" << std::endl;
        else
            std::cerr << "filepath " << filePath << "   false" << std::endl;
            
        if ((filePath.find("cgi") != std::string::npos || request.hasFileSpecialRoute(getExtension(filePath)))) 
        {
            std::cerr << "in CGI" << std::endl;   
            Location *route = NULL;
            if (request.hasFileSpecialRoute(getExtension(filePath)))
                route = server->getRoute(getExtension(filePath));
            if (filePath.find("cgi") != std::string::npos)
                route = server->getRoute("/cgi-bin/");
            if (!route)
            {
                std::cerr << "no route" << std::endl;
                return;
            }
            else if (response.isAllowedMethod(route, request))
            {
                std::cerr << "in CGI" << std::endl;   
                Cgi_handler cgiHandler;
                cgiHandler.executeCGI(filePath, request, *fd_client);
                std::cerr << "CGI executed" << std::endl;                
            }
            else
            {
                 std::cerr << "22222222222" << std::endl;   
                response.setRedirection(406);
                response.send(*fd_client);
            }
        }
        else if (filePath.find("cgi-bin/") != std::string::npos && !server->getCgiStatus())
        {
             std::cerr << "3333333333333" << std::endl;   
            response.setRedirection(403);
            response.send(*fd_client);
        }
        else 
        {
            std::cerr << "4444444444444" << std::endl;   
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
    // }
    // else
    //     Log::output("./logs/error.log") << "Requête incomplète : en attente de plus de données." << std::endl;

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

void saveRawRequestToFile(const std::string& data) {
    std::ofstream file("./sessions/request.txt", std::ios::app); // Mode append
    if (file.is_open()) {
        file << "===== Nouvelle Requête =====\n";
        file << data; // Écrit directement les données reçues
        file << "\n===== Fin de la Requête =====\n";
        file.close();
    } else {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier ./sessions/request.txt\n";
    }
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
                    std::cerr << "Signal reçu. Arrêt en cours." << std::endl;
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
                        Log::error("Erreur lors de accept");
                        continue;
                    }
                    epoll.makeSocketNonBlocking(fd_client);
                    epoll.addFd(fd_client, EPOLLIN);
                    std::cerr << "Nouveau client accepté : FD " << fd_client << std::endl;
                }
                else if (event.events & EPOLLIN) 
                {
                    fd_client = event.data.fd;
                    std::cerr << "FD " << fd_client << " est prêt pour recv." << std::endl;

                    std::string &currentRequest = requestMap[fd_client];

                    while ((reads = recv(fd_client, buff, BUFFER_SIZE, 0)) > 0)
                    {
                        currentRequest.append(buff, reads);
                        std::cerr << "Reçu " << reads << " octets sur FD " << fd_client << ". Taille accumulée : " << currentRequest.size() << " octets." << std::endl;
                    }

                    if (reads == 0) 
                    {
                        std::cerr << "Connexion fermée par le client (FD " << fd_client << ")." << std::endl;
                        epoll.removeFd(fd_client);
                        close(fd_client);
                        requestMap.erase(fd_client);
                        continue;
                    }
                    else if (reads < 0 && errno != EAGAIN && errno != EWOULDBLOCK) 
                    {
                        std::cerr << "Erreur lors de recv (FD " << fd_client << ") : " << strerror(errno) << std::endl;
                        epoll.removeFd(fd_client);
                        close(fd_client);
                        requestMap.erase(fd_client);
                        continue;
                    }


                    
                    size_t headerEnd = currentRequest.find("\r\n\r\n");
                    if (headerEnd != std::string::npos)
                    {
                        std::cerr << "En-têtes complets reçus sur FD " << fd_client << "." << std::endl;

                        
                        if (currentRequest.find("Transfer-Encoding: chunked") != std::string::npos)
                        {
                            std::cerr << "Détection de Transfer-Encoding: chunked sur FD " << fd_client << "." << std::endl;

                            size_t chunk_start = headerEnd + 4;
                            while (true)
                            {
                                size_t chunk_size_end = currentRequest.find("\r\n", chunk_start);
                                if (chunk_size_end == std::string::npos)
                                {
                                    std::cerr << "Chunk incomplet détecté sur FD " << fd_client << "." << std::endl;
                                    break;
                                }

                                std::string chunk_size_str = currentRequest.substr(chunk_start, chunk_size_end - chunk_start);
                                size_t chunk_size = std::strtol(chunk_size_str.c_str(), NULL, 16);

                                if (chunk_size == 0)
                                {
                                    std::cerr << "Chunk final reçu sur FD " << fd_client << "." << std::endl;
                                    int serverIndex = findServerIndex(currentRequest, Servers);
                                    type_request_manager(&fd_client, &currentRequest, Servers[serverIndex], &epoll, sessionManager);
                                    currentRequest.clear();
                                    break;
                                }

                                size_t chunk_data_start = chunk_size_end + 2;
                                size_t chunk_data_end = chunk_data_start + chunk_size;

                                if (chunk_data_end > currentRequest.size())
                                {
                                    std::cerr << "Données chunk incompletes détectées sur FD " << fd_client << "." << std::endl;
                                    break;
                                }

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
                                        std::cerr << "Requête complète reçue sur FD " << fd_client << "." << std::endl;
                                        int serverIndex = findServerIndex(currentRequest, Servers);
                                        type_request_manager(&fd_client, &currentRequest, Servers[serverIndex], &epoll, sessionManager);
                                        currentRequest.clear();
                                    }
                                    else
                                    {
                                        std::cerr << "Contenu incomplet sur FD " << fd_client << ". Attente de plus de données." << std::endl;
                                    }
                                }
                            }
                            else
                            {
                                
                                std::cerr << "Pas de Content-Length ni chunked. Traitement en tant que requête simple." << std::endl;
                                int serverIndex = findServerIndex(currentRequest, Servers);
                                type_request_manager(&fd_client, &currentRequest, Servers[serverIndex], &epoll, sessionManager);
                                currentRequest.clear();
                            }
                        }
                    }
                }
            }
        }

        if (signalReceived)
        {
            Log::cleanup();
            throw SignalException();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Erreur capturée : " << e.what() << std::endl;
    }
}



/**
 * @brief Version fonctionnel complete de la fonction request_and_response_fd_manager
 */

// void request_and_response_fd_manager(std::vector<int>& fd_sockets, std::vector<Server*>& Servers, SessionManager &sessionManager)
// {
//     try
//     {
//         std::map<int, std::string> requestMap;
//         char buff[BUFFER_SIZE];
//         int fd_client;
//         ssize_t reads;
//         struct sockaddr_in client_addr;
//         socklen_t client_addr_len;

//         fd_set read_fds;
//         int max_fd = -1;

        
//         std::vector<int> listen_sockets = fd_sockets;

//         std::cerr << "Initialisation de request_and_response_fd_manager..." << std::endl;

        
//         for (size_t i = 0; i < fd_sockets.size(); ++i)
//         {
//             if (fd_sockets[i] > max_fd)
//                 max_fd = fd_sockets[i];
//         }
//         std::cerr << "Max FD initial : " << max_fd << std::endl;

//         while (!signalReceived)
//         {
//             FD_ZERO(&read_fds);

            
//             for (size_t i = 0; i < fd_sockets.size(); ++i)
//             {
//                 FD_SET(fd_sockets[i], &read_fds);
//                 std::cerr << "Ajout de FD " << fd_sockets[i] << " à l'ensemble." << std::endl;
//             }

            
//             FD_SET(signalPipeFd[0], &read_fds);
//             if (signalPipeFd[0] > max_fd)
//                 max_fd = signalPipeFd[0];
//             std::cerr << "Ajout du signalPipeFd[0] (" << signalPipeFd[0] << ") à l'ensemble. Max FD : " << max_fd << std::endl;

            
//             int eventCount = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
//             if (eventCount == -1)
//             {
//                 std::cerr << "Erreur dans select : " << strerror(errno) << std::endl;
//                 break;
//             }
//             std::cerr << "Select a détecté " << eventCount << " événement(s)." << std::endl;

            
//             if (FD_ISSET(signalPipeFd[0], &read_fds))
//             {
//                 std::cerr << "Signal reçu. Fermeture en cours..." << std::endl;
//                 signalReceived = true;
//                 break;
//             }

            
//             std::vector<int> to_remove;

            
//             for (size_t i = 0; i < fd_sockets.size(); ++i)
//             {
//                 fd_client = fd_sockets[i];
//                 std::cerr << "Traitement de FD " << fd_client << "..." << std::endl;

                
//                 if (std::find(listen_sockets.begin(), listen_sockets.end(), fd_client) != listen_sockets.end() && FD_ISSET(fd_client, &read_fds))
//                 {
//                     std::cerr << "FD " << fd_client << " est un socket d'écoute prêt pour accept." << std::endl;
//                     client_addr_len = sizeof(client_addr);
//                     int new_client = accept(fd_client, (struct sockaddr*)&client_addr, &client_addr_len);
//                     if (new_client == -1)
//                     {
//                         std::cerr << "Erreur dans accept : " << strerror(errno) << std::endl;
//                         continue;
//                     }
//                     std::cerr << "Nouveau client accepté : FD " << new_client << std::endl;

                    
//                     fd_sockets.push_back(new_client);
//                     if (new_client > max_fd)
//                         max_fd = new_client;
//                     continue;
//                 }
//                 else if (FD_ISSET(fd_client, &read_fds)) 
//                 {
//                     std::cerr << "FD " << fd_client << " est prêt pour recv." << std::endl;
//                     reads = recv(fd_client, buff, BUFFER_SIZE, 0);

//                     if (reads > 0)
//                     {
//                         requestMap[fd_client].append(buff, reads);
//                         std::cerr << "Données reçues sur FD " << fd_client << " : " << reads << " octets." << std::endl;

                        
//                         size_t headerEnd = requestMap[fd_client].find("\r\n\r\n");
//                         if (headerEnd != std::string::npos)
//                         {
//                             std::cerr << "En-têtes complets reçus sur FD " << fd_client << "." << std::endl;

                            
//                             if (requestMap[fd_client].find("Transfer-Encoding: chunked") != std::string::npos)
//                             {
//                                 std::cerr << "Requête avec Transfer-Encoding: chunked détectée sur FD " << fd_client << "." << std::endl;

                                
//                                 size_t chunk_start = headerEnd + 4;
//                                 while (true)
//                                 {
//                                     size_t chunk_size_end = requestMap[fd_client].find("\r\n", chunk_start);
//                                     if (chunk_size_end == std::string::npos)
//                                     {
//                                         std::cerr << "Chunk incomplet détecté sur FD " << fd_client << "." << std::endl;
//                                         break;
//                                     }

//                                     std::string chunk_size_str = requestMap[fd_client].substr(chunk_start, chunk_size_end - chunk_start);
//                                     size_t chunk_size = std::strtol(chunk_size_str.c_str(), NULL, 16);

//                                     if (chunk_size == 0)
//                                     {
//                                         std::cerr << "Chunk final détecté sur FD " << fd_client << "." << std::endl;
//                                         int serverIndex = findServerIndex(requestMap[fd_client], Servers);
//                                         type_request_manager(&fd_client, &requestMap[fd_client], Servers[serverIndex], NULL, sessionManager);
//                                         requestMap[fd_client].clear();
//                                         break;
//                                     }

//                                     size_t chunk_data_start = chunk_size_end + 2;
//                                     size_t chunk_data_end = chunk_data_start + chunk_size;

//                                     if (chunk_data_end > requestMap[fd_client].size())
//                                     {
//                                         std::cerr << "Données de chunk incomplètes détectées sur FD " << fd_client << "." << std::endl;
//                                         break;
//                                     }

//                                     chunk_start = chunk_data_end + 2; 
//                                 }
//                             }
//                             else
//                             {
                                
//                                 size_t content_length_pos = requestMap[fd_client].find("Content-Length:");
//                                 if (content_length_pos != std::string::npos)
//                                 {
//                                     std::cerr << "Content-Length détecté sur FD " << fd_client << "." << std::endl;
//                                     size_t content_length_start = content_length_pos + strlen("Content-Length:");
//                                     size_t content_length_end = requestMap[fd_client].find("\r\n", content_length_start);

//                                     if (content_length_end != std::string::npos)
//                                     {
//                                         std::string content_length_str = requestMap[fd_client].substr(content_length_start, content_length_end - content_length_start);
//                                         size_t content_length = std::atoi(content_length_str.c_str());
//                                         size_t body_start = headerEnd + 4;

//                                         if (requestMap[fd_client].size() >= body_start + content_length)
//                                         {
//                                             std::cerr << "Requête complète détectée sur FD " << fd_client << "." << std::endl;
//                                             int serverIndex = findServerIndex(requestMap[fd_client], Servers);
//                                             type_request_manager(&fd_client, &requestMap[fd_client], Servers[serverIndex], NULL, sessionManager);
//                                             requestMap[fd_client].clear();
//                                         }
//                                         else
//                                         {
//                                             std::cerr << "Corps incomplet détecté sur FD " << fd_client << ". Attente de plus de données." << std::endl;
//                                         }
//                                     }
//                                 }
//                                 else
//                                 {
                                    
//                                     std::cerr << "Pas de Content-Length ni Transfer-Encoding détecté. Traitement comme requête simple." << std::endl;
//                                     int serverIndex = findServerIndex(requestMap[fd_client], Servers);
//                                     type_request_manager(&fd_client, &requestMap[fd_client], Servers[serverIndex], NULL, sessionManager);
//                                     requestMap[fd_client].clear();
//                                 }
//                             }
//                         }
//                     }
//                     else if (reads == 0) 
//                     {
//                         std::cerr << "Connexion fermée par le client (FD " << fd_client << ")." << std::endl;
//                         close(fd_client);
//                         to_remove.push_back(fd_client);
//                     }
//                     else if (reads < 0 && errno != EAGAIN && errno != EWOULDBLOCK) 
//                     {
//                         std::cerr << "Erreur dans recv (FD " << fd_client << ") : " << strerror(errno) << std::endl;
//                         close(fd_client);
//                         to_remove.push_back(fd_client);
//                     }
//                 }

//             }

            
//             for (size_t j = 0; j < to_remove.size(); ++j)
//             {
//                 for (std::vector<int>::iterator it = fd_sockets.begin(); it != fd_sockets.end(); ++it)
//                 {
//                     if (*it == to_remove[j])
//                     {
//                         std::cerr << "Suppression de FD " << *it << " de la liste." << std::endl;
//                         fd_sockets.erase(it);
//                         break;
//                     }
//                 }
//             }
//         }

//         if (signalReceived)
//         {
//             Log::cleanup();
//             throw SignalException();
//         }
//     }
//     catch (std::exception& e)
//     {
//         std::cerr << "Exception capturée : " << e.what() << std::endl;
//     }
// }



/**
 * @brief Version avec select de la fonction request_and_response_fd_manager
 */

// void request_and_response_fd_manager(std::vector<int>& fd_sockets, std::vector<Server*>& Servers, SessionManager &sessionManager)
// {
//     try
//     {
//         std::map<int, std::string> requestMap;
//         char buff[BUFFER_SIZE];
//         int fd_client;
//         ssize_t reads;
//         struct sockaddr_in client_addr;
//         socklen_t client_addr_len;

//         fd_set read_fds;
//         int max_fd = -1;

//         // Liste distincte pour les sockets d'écoute
//         std::vector<int> listen_sockets = fd_sockets;

//         std::cerr << "Initialisation de request_and_response_fd_manager..." << std::endl;

//         // Trouver le plus grand descripteur
//         for (size_t i = 0; i < fd_sockets.size(); ++i)
//         {
//             if (fd_sockets[i] > max_fd)
//                 max_fd = fd_sockets[i];
//         }
//         std::cerr << "Max FD initial : " << max_fd << std::endl;

//         while (!signalReceived)
//         {
//             FD_ZERO(&read_fds);

//             // Ajouter tous les sockets (écoute + clients) à l'ensemble de lecture
//             for (size_t i = 0; i < fd_sockets.size(); ++i)
//             {
//                 FD_SET(fd_sockets[i], &read_fds);
//                 std::cerr << "Ajout de FD " << fd_sockets[i] << " à l'ensemble." << std::endl;
//             }

//             // Ajouter le pipe pour les signaux
//             FD_SET(signalPipeFd[0], &read_fds);
//             if (signalPipeFd[0] > max_fd)
//                 max_fd = signalPipeFd[0];
//             std::cerr << "Ajout du signalPipeFd[0] (" << signalPipeFd[0] << ") à l'ensemble. Max FD : " << max_fd << std::endl;

//             // Attendre les événements
//             int eventCount = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
//             if (eventCount == -1)
//             {
//                 std::cerr << "Erreur dans select : " << strerror(errno) << std::endl;
//                 break;
//             }
//             std::cerr << "Select a détecté " << eventCount << " événement(s)." << std::endl;

//             // Vérifier les signaux
//             if (FD_ISSET(signalPipeFd[0], &read_fds))
//             {
//                 std::cerr << "Signal reçu. Fermeture en cours..." << std::endl;
//                 signalReceived = true;
//                 break;
//             }

//             // Liste temporaire pour suppression des sockets invalides
//             std::vector<int> to_remove;

//             // Parcourir les sockets actifs
//             for (size_t i = 0; i < fd_sockets.size(); ++i)
//             {
//                 fd_client = fd_sockets[i];
//                 std::cerr << "Traitement de FD " << fd_client << "..." << std::endl;

//                 // Si c'est un socket d'écoute
//                 if (std::find(listen_sockets.begin(), listen_sockets.end(), fd_client) != listen_sockets.end() && FD_ISSET(fd_client, &read_fds))
//                 {
//                     std::cerr << "FD " << fd_client << " est un socket d'écoute prêt pour accept." << std::endl;
//                     client_addr_len = sizeof(client_addr);
//                     int new_client = accept(fd_client, (struct sockaddr*)&client_addr, &client_addr_len);
//                     if (new_client == -1)
//                     {
//                         std::cerr << "Erreur dans accept : " << strerror(errno) << std::endl;
//                         continue;
//                     }
//                     std::cerr << "Nouveau client accepté : FD " << new_client << std::endl;

//                     // Ajouter le nouveau client à la liste
//                     fd_sockets.push_back(new_client);
//                     if (new_client > max_fd)
//                         max_fd = new_client;
//                     continue;
//                 }
//                 else if (FD_ISSET(fd_client, &read_fds)) // Si c'est un socket client
//                 {
//                     std::cerr << "FD " << fd_client << " est prêt pour recv." << std::endl;
//                     reads = recv(fd_client, buff, BUFFER_SIZE, 0);
//                     if (reads > 0)
//                     {
//                         requestMap[fd_client].append(buff, reads);
//                         std::cerr << "Données reçues sur FD " << fd_client << " : " << reads << " octets." << std::endl;

//                         // Vérifiez si la requête est complète (fin des en-têtes HTTP)
//                         size_t headerEnd = requestMap[fd_client].find("\r\n\r\n");
//                         if (headerEnd != std::string::npos)
//                         {
//                             /**
//                              * Debug message
//                              */
//                             std::map<int, std::string>::iterator it = requestMap.begin();
//                             while (it != requestMap.end())
//                             {
//                                 std::cerr << GREEN << "fd_client: " << it->first << RESET << std::endl;

//                                 // Conversion de it->first (int) en chaîne
//                                 std::ostringstream oss;
//                                 oss << it->first;

//                                 // Construction du chemin du fichier
//                                 std::string file = "./sessions/request_" + oss.str() + ".txt";

//                                 std::cerr << "File path: " << file << std::endl; // Debugging log

//                                 std::ofstream fileStream(file.c_str());

//                                 fileStream << it->second;
//                                 fileStream.close();
                                
//                                 it++;
//                             }
//                             std::cerr << "Requête complète reçue sur FD " << fd_client << ". Traitement..." << std::endl;
//                             int serverIndex = findServerIndex(requestMap[fd_client], Servers);
//                             type_request_manager(&fd_client, &requestMap[fd_client], Servers[serverIndex], NULL, sessionManager);

//                             // Nettoyage de la requête après traitement
//                             requestMap[fd_client].clear();
//                         }
//                     }
//                     else if (reads == 0) // Connexion fermée
//                     {
//                         std::cerr << "Connexion fermée par le client (FD " << fd_client << ")." << std::endl;
//                         close(fd_client);
//                         to_remove.push_back(fd_client);
//                     }
//                     else if (reads < 0 && errno != EAGAIN && errno != EWOULDBLOCK) // Erreur
//                     {
//                         std::cerr << "Erreur dans recv (FD " << fd_client << ") : " << strerror(errno) << std::endl;
//                         close(fd_client);
//                         to_remove.push_back(fd_client);
//                     }
//                 }
//             }

//             // Supprimer les sockets invalides après l'itération
//             for (size_t j = 0; j < to_remove.size(); ++j)
//             {
//                 for (std::vector<int>::iterator it = fd_sockets.begin(); it != fd_sockets.end(); ++it)
//                 {
//                     if (*it == to_remove[j])
//                     {
//                         std::cerr << "Suppression de FD " << *it << " de la liste." << std::endl;
//                         fd_sockets.erase(it);
//                         break;
//                     }
//                 }
//             }
//         }

//         if (signalReceived)
//         {
//             Log::cleanup();
//             throw SignalException();
//         }
//     }
//     catch (std::exception& e)
//     {
//         std::cerr << "Exception capturée : " << e.what() << std::endl;
//     }
// }


/***
 * 
 * @brief Version fonctionelle avec epoll de la fonction request_and_response_fd_manager
 */


// void saveRawRequestToFile(const std::string& data) {
//     std::ofstream file("./sessions/request.txt", std::ios::app); // Mode append
//     if (file.is_open()) {
//         file << "===== Nouvelle Requête =====\n";
//         file << data; // Écrit directement les données reçues
//         file << "\n===== Fin de la Requête =====\n";
//         file.close();
//     } else {
//         std::cerr << "Erreur : Impossible d'ouvrir le fichier ./sessions/request.txt\n";
//     }
// }


// void request_and_response_fd_manager(std::vector<int>& fd_sockets, std::vector<Server*>& Servers, SessionManager &sessionManager)
// {
//     try
//     {
//         std::map<int, std::string> requestMap;
//         Epoll epoll(10);
//         char buff[BUFFER_SIZE];
//         int fd_client;
//         ssize_t reads;
//         struct epoll_event event;
//         struct sockaddr_in client_addr;
//         socklen_t client_addr_len;
//         std::string req;


//         for (size_t i = 0; i < fd_sockets.size(); ++i)
//         {
//             epoll.addFd(fd_sockets[i], EPOLLIN);
//             epoll.makeSocketNonBlocking(fd_sockets[i]);
//         }
        
//         epoll.addFd(signalPipeFd[0], EPOLLIN);

//         while (!signalReceived) 
//         {
//             int eventCount = epoll.wait(-1);
//             if (eventCount == -1)
//             {
//                 Log::error("Fatal error during epoll_wait");
//                 break;
//             }

//             if (eventCount == 0)
//                 continue;

//             for (int i = 0; i < eventCount && !signalReceived; ++i) 
//             {
//                 Log::debug("Event ***********************");
//                 event = epoll.getEvent(i);
//                 Log::debug("*****************************");
//                 bool isServerSocket = false;

//                 for (size_t j = 0; j < fd_sockets.size(); ++j)
//                 {
//                     if (event.data.fd == fd_sockets[j])
//                     {
//                         isServerSocket = true;
//                         break;
//                     }
//                 }
//                 if (isServerSocket)
//                 {
//                     client_addr_len = sizeof(client_addr);
//                     fd_client = accept(event.data.fd, (struct sockaddr*)&client_addr, &client_addr_len);
//                     if (fd_client == -1) 
//                     {
//                         Log::error("accept");
//                         continue;
//                     }
//                     epoll.makeSocketNonBlocking(fd_client);
//                     epoll.addFd(fd_client, EPOLLIN);
//                 }
//                 else if (event.events & EPOLLIN) 
//                 {
//                     ///////////////////////
//                     fd_client = event.data.fd;                    
                    
                    
//                     // while (true) 
//                     // {
//                     //     reads = recv(fd_client, buff, BUFFER_SIZE, 0);
//                     //     if (reads <= 0)
//                     //         break;
//                     //     requestMap[fd_client].append(buff, reads);
//                     // }
                    
//                     while (true) 
//                     {
//                         reads = recv(fd_client, buff, BUFFER_SIZE, 0);
//                         if (reads > 0)
//                         {
//                             requestMap[fd_client].append(buff, reads);
//                             std::cerr << "Bytes reçus : " << reads << ", Taille totale accumulée : " << requestMap[fd_client].size() << std::endl;
//                         }
//                         else if (reads == 0)
//                         {
//                             std::cerr << "Connexion fermée par le client (fd=" << fd_client << ")" << std::endl;
//                             break;
//                         }
//                         else if (errno == EAGAIN || errno == EWOULDBLOCK)
//                         {
//                             std::cerr << "Pas de données supplémentaires pour fd=" << fd_client << std::endl;
//                             break;
//                         }
//                         else
//                         {
//                             std::cerr << "Erreur dans recv (fd=" << fd_client << "): " << strerror(errno) << std::endl;
//                             break;
//                         }
//                     }


//                     // while (true) 
//                     // {
//                     //     reads = recv(fd_client, buff, BUFFER_SIZE, 0);
//                     //     if (reads <= 0)
//                     //         break;
//                     //     requestMap[fd_client].append(buff, reads);
//                     // }

                    
//                     std::cerr << RED << "Bytes received in recv(): " << reads << RESET << std::endl;
//                     std::cerr << RED << "requestMap[" << fd_client << "].size() : " << requestMap[fd_client].size() << RESET << std::endl;

//                     saveRawRequestToFile(requestMap[fd_client]);
//                     std::cerr << RED << "req.size() : " << requestMap[fd_client].size() << RESET << std::endl;

                    
//                     int serverIndex = findServerIndex(requestMap[fd_client], Servers);
//                     size_t headerEnd = requestMap[fd_client].find("\r\n\r\n");
//                     if (headerEnd != std::string::npos) 
//                     {

//                         /**
//                          * Debug message
//                          */
//                         std::map<int, std::string>::iterator it = requestMap.begin();
//                         while (it != requestMap.end())
//                         {
//                             std::cerr << GREEN << "fd_client: " << it->first << RESET << std::endl;

//                             // Conversion de it->first (int) en chaîne
//                             std::ostringstream oss;
//                             oss << it->first;

//                             // Construction du chemin du fichier
//                             std::string file = "./sessions/request_" + oss.str() + ".txt";

//                             std::cerr << "File path: " << file << std::endl; // Debugging log

//                             std::ofstream fileStream(file.c_str());

//                             fileStream << it->second;
//                             fileStream.close();
                            
//                             it++;
//                         }
//                         // std::map<int, std::string>::iterator it = requestMap.begin();
//                         // while (it != requestMap.end())
//                         // {
//                         //     std::cerr << GREEN << "fd_client: " << it->first << RESET << std::endl;

//                         //     // Conversion de it->first (int) en chaîne
//                         //     std::ostringstream oss;
//                         //     oss << it->first;

//                         //     // Construction du chemin du fichier
//                         //     std::string file = "./sessions/request_" + oss.str() + ".txt";
//                         //     std::cerr << "File path: " << file << std::endl; // Debugging log

//                         //     // Ouvrir un fichier pour écrire les données lues
//                         //     std::ofstream fileStream(file.c_str());
//                         //     if (!fileStream.is_open())
//                         //     {
//                         //         std::cerr << "Erreur : Impossible d'ouvrir le fichier " << file << std::endl;
//                         //         ++it;
//                         //         continue;
//                         //     }

//                         //     // Lire depuis le descripteur it->first
//                         //     char buffer[1024];
//                         //     ssize_t bytesRead;
//                         //     while ((bytesRead = read(it->first, buffer, sizeof(buffer))) > 0)
//                         //     {
//                         //         fileStream.write(buffer, bytesRead); // Écrire les données dans le fichier
//                         //     }

//                         //     if (bytesRead == -1)
//                         //     {
//                         //         std::cerr << "Erreur lors de la lecture depuis fd " << it->first << ": " << strerror(errno) << std::endl;
//                         //     }

//                         //     fileStream.close();
//                         //     ++it;
//                         // }

//                         ////////////////
                        
//                         type_request_manager(&fd_client, &requestMap[fd_client], Servers[serverIndex], &epoll, sessionManager);
//                     }
//                     ///////////////////////
//                 }
//             }
//         }
        
        
//         if (signalReceived)
//         {
//             //sessionManager.saveSessionsToFile();
//             Log::cleanup();
//             throw SignalException();
//         }
//     }
//     catch (std::exception& e)
//     {
//         std::cerr << "Error: " << e.what() << std::endl;
//     }
// }

/**
 * @brief Version fonctionnelle avec epoll de la fonction request_and_response_fd_manager
 */


// void request_and_response_fd_manager(std::vector<int>& fd_sockets, std::vector<Server*>& Servers, SessionManager &sessionManager)
// {
//     try
//     {
//         std::map<int, std::string> requestMap;
//         Epoll epoll(10);
//         char buff[BUFFER_SIZE];
//         int fd_client;
//         ssize_t reads;
//         struct epoll_event event;
//         struct sockaddr_in client_addr;
//         socklen_t client_addr_len;
//         std::string req;


//         for (size_t i = 0; i < fd_sockets.size(); ++i)
//         {
//             epoll.addFd(fd_sockets[i], EPOLLIN);
//             epoll.makeSocketNonBlocking(fd_sockets[i]);
//         }
        
//         epoll.addFd(signalPipeFd[0], EPOLLIN);

//         while (!signalReceived) 
//         {
//             int eventCount = epoll.wait(-1);
//             if (eventCount == -1)
//             {
//                 Log::error("Fatal error during epoll_wait");
//                 break;
//             }

//             if (eventCount == 0)
//                 continue;

//             for (int i = 0; i < eventCount && !signalReceived; ++i) 
//             {
//                 Log::debug("Event ***********************");
//                 event = epoll.getEvent(i);
//                 Log::debug("*****************************");
//                 bool isServerSocket = false;

//                 for (size_t j = 0; j < fd_sockets.size(); ++j)
//                 {
//                     if (event.data.fd == fd_sockets[j])
//                     {
//                         isServerSocket = true;
//                         break;
//                     }
//                 }
//                 if (isServerSocket)
//                 {
//                     client_addr_len = sizeof(client_addr);
//                     fd_client = accept(event.data.fd, (struct sockaddr*)&client_addr, &client_addr_len);
//                     if (fd_client == -1) 
//                     {
//                         Log::error("accept");
//                         continue;
//                     }
//                     epoll.makeSocketNonBlocking(fd_client);
//                     epoll.addFd(fd_client, EPOLLIN);
//                 }
//                 else if (event.events & EPOLLIN) 
//                 {
//                     ///////////////////////
//                     fd_client = event.data.fd;                    
                    
                    
//                     while (true) 
//                     {
//                         reads = recv(fd_client, buff, BUFFER_SIZE, 0);
//                         if (reads <= 0)
//                             break;
//                         requestMap[fd_client].append(buff, reads);
//                     }

//                     // while (true) 
//                     // {
//                     //     reads = recv(fd_client, buff, BUFFER_SIZE, 0);
//                     //     if (reads <= 0)
//                     //         break;
//                     //     requestMap[fd_client].append(buff, reads);
//                     // }

                    
//                     std::cerr << RED << "Bytes received in recv(): " << reads << RESET << std::endl;
//                     std::cerr << RED << "requestMap[" << fd_client << "].size() : " << requestMap[fd_client].size() << RESET << std::endl;

//                     saveRawRequestToFile(requestMap[fd_client]);
//                     std::cerr << RED << "req.size() : " << requestMap[fd_client].size() << RESET << std::endl;

                    
//                     int serverIndex = findServerIndex(requestMap[fd_client], Servers);
//                     size_t headerEnd = requestMap[fd_client].find("\r\n\r\n");
//                     if (headerEnd != std::string::npos) 
//                     {

//                         /**
//                          * Debug message
//                          */
//                         std::map<int, std::string>::iterator it = requestMap.begin();
//                         while (it != requestMap.end())
//                         {
//                             std::cerr << GREEN << "fd_client: " << it->first << RESET << std::endl;

//                             // Conversion de it->first (int) en chaîne
//                             std::ostringstream oss;
//                             oss << it->first;

//                             // Construction du chemin du fichier
//                             std::string file = "./sessions/request_" + oss.str() + ".txt";

//                             std::cerr << "File path: " << file << std::endl; // Debugging log

//                             std::ofstream fileStream(file.c_str());

//                             fileStream << it->second;
//                             fileStream.close();
                            
//                             it++;
//                         }
//                         // std::map<int, std::string>::iterator it = requestMap.begin();
//                         // while (it != requestMap.end())
//                         // {
//                         //     std::cerr << GREEN << "fd_client: " << it->first << RESET << std::endl;

//                         //     // Conversion de it->first (int) en chaîne
//                         //     std::ostringstream oss;
//                         //     oss << it->first;

//                         //     // Construction du chemin du fichier
//                         //     std::string file = "./sessions/request_" + oss.str() + ".txt";
//                         //     std::cerr << "File path: " << file << std::endl; // Debugging log

//                         //     // Ouvrir un fichier pour écrire les données lues
//                         //     std::ofstream fileStream(file.c_str());
//                         //     if (!fileStream.is_open())
//                         //     {
//                         //         std::cerr << "Erreur : Impossible d'ouvrir le fichier " << file << std::endl;
//                         //         ++it;
//                         //         continue;
//                         //     }

//                         //     // Lire depuis le descripteur it->first
//                         //     char buffer[1024];
//                         //     ssize_t bytesRead;
//                         //     while ((bytesRead = read(it->first, buffer, sizeof(buffer))) > 0)
//                         //     {
//                         //         fileStream.write(buffer, bytesRead); // Écrire les données dans le fichier
//                         //     }

//                         //     if (bytesRead == -1)
//                         //     {
//                         //         std::cerr << "Erreur lors de la lecture depuis fd " << it->first << ": " << strerror(errno) << std::endl;
//                         //     }

//                         //     fileStream.close();
//                         //     ++it;
//                         // }

//                         ////////////////
                        
//                         type_request_manager(&fd_client, &requestMap[fd_client], Servers[serverIndex], &epoll, sessionManager);
//                     }
//                     ///////////////////////
//                 }
//             }
//         }
        
        
//         if (signalReceived)
//         {
//             //sessionManager.saveSessionsToFile();
//             Log::cleanup();
//             throw SignalException();
//         }
//     }
//     catch (std::exception& e)
//     {
//         std::cerr << "Error: " << e.what() << std::endl;
//     }
// }



/**
 * @brief Version originale de la fonction request_and_response_fd_manager
 */



// void request_and_response_fd_manager(std::vector<int>& fd_sockets, std::vector<Server*>& Servers, SessionManager &sessionManager)
// {
//     try
//     {
//         std::map<int, std::string> requestMap;
//         Epoll epoll(10);
//         char buff[BUFFER_SIZE];
//         int fd_client;
//         int reads;
//         struct epoll_event event;
//         struct sockaddr_in client_addr;
//         socklen_t client_addr_len;

//         for (size_t i = 0; i < fd_sockets.size(); ++i)
//         {
//             epoll.addFd(fd_sockets[i], EPOLLIN);
//             epoll.makeSocketNonBlocking(fd_sockets[i]);
//         }
        
//         epoll.addFd(signalPipeFd[0], EPOLLIN);

//         while (!signalReceived) 
//         {
//             int eventCount = epoll.wait(-1);
//             if (eventCount == -1)
//             {
//                 Log::error("Fatal error during epoll_wait");
//                 break;
//             }

//             if (eventCount == 0)
//                 continue;

//             for (int i = 0; i < eventCount && !signalReceived; ++i) 
//             {
//                 event = epoll.getEvent(i);
//                 bool isServerSocket = false;

//                 for (size_t j = 0; j < fd_sockets.size(); ++j)
//                 {
//                     if (event.data.fd == fd_sockets[j])
//                     {
//                         isServerSocket = true;
//                         break;
//                     }
//                 }
//                 if (isServerSocket)
//                 {
//                     client_addr_len = sizeof(client_addr);
//                     fd_client = accept(event.data.fd, (struct sockaddr*)&client_addr, &client_addr_len);
//                     if (fd_client == -1) 
//                     {
//                         Log::error("accept");
//                         continue;
//                     }
//                     epoll.makeSocketNonBlocking(fd_client);
//                     epoll.addFd(fd_client, EPOLLIN | EPOLLET);
//                 }
//                 else if (event.events & EPOLLIN) 
//                 {
//                     fd_client = event.data.fd;
//                     while ((reads = read(fd_client, buff, BUFFER_SIZE)) > 0) 
//                     {
//                         requestMap[fd_client] += std::string(buff, reads);
//                         bzero(buff, BUFFER_SIZE);
//                     }
                    
//                     std::string& req = requestMap[fd_client];
//                     size_t headerEndPos = req.find("\r\n\r\n");
//                     ssize_t contentLength = -1;
//                     if (headerEndPos != std::string::npos) 
//                     {
//                         size_t contentLengthPos = req.find("Content-Length: ");
//                         if (contentLengthPos != std::string::npos) 
//                         {
//                             contentLengthPos += 16;
//                             size_t endPos = req.find("\r\n", contentLengthPos);
//                             std::string contentLengthStr = req.substr(contentLengthPos, endPos - contentLengthPos);
//                             std::istringstream iss(contentLengthStr);
//                             iss >> contentLength;
//                         }
//                         ssize_t totalRead = req.size();
                        
//                         if (contentLength == -1 || totalRead >= (ssize_t)headerEndPos + 4 + contentLength) 
//                         {
//                             int serverIndex = findServerIndex(req, Servers);                
//                             type_request_manager(&fd_client, &req, Servers[serverIndex], &epoll, sessionManager);                        
//                             requestMap.erase(fd_client);
//                             //close(fd_client);
//                         }
//                     }
//                     if (reads == 0) 
                    
//                     {
//                         //epoll.removeFd(fd_client);
//                         //close(fd_client);
//                         requestMap.erase(fd_client);
//                     }
//                 }
//             }
//         }
//         if (signalReceived)
//         {
//             //sessionManager.saveSessionsToFile();
//             Log::cleanup();
//             throw SignalException();
//         }
//     }
//     catch (std::exception& e)
//     {
//         std::cerr << "Error: " << e.what() << std::endl;
//     }
// }
