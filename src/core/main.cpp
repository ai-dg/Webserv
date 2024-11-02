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
#include "../headers/Server.hpp"
#include "../headers/colors.hpp"
#include "../headers/files.hpp"
#include "../headers/parser.hpp"
#include "../headers/HttpRequest.hpp"
#include "../headers/HttpResponse.hpp"
#include "../headers/Conf.hpp"
#include "../headers/cgi_handler.hpp"
#include "../headers/Epoll.hpp"
#include "../headers/Log.hpp"
#include "../headers/ipTools.hpp"
#include "../headers/SessionManager.hpp"
#include "../headers/Cookies.hpp"

#define BUFFER_SIZE 2048

int socket_start(std::vector<int>& fd_sockets, std::vector<int>& listPorts)
{
    /**
    *    int fd_socket = socket(AF_INET, SOCK_STREAM, 0);
    *    Creation d'un socket permettant la connextion
    *    l'option AF_INET permet de choisir le protocole de connexion Protocoles Internet IPv4
    *    l'option SOCK_STREAM permet de choisir le type de connexion TCP man : (
    *    SOCK_STREAM Support de dialogue garantissant l'intégrité, fournissant un flux de données binaires, 
    *    et intégrant un mécanisme pour les transmissions de données hors-bande. )
    */

    fd_sockets.clear();
    for (int i = 0; i < listPorts.size(); ++i)
    {
        int fd_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (fd_socket == -1)
        {
            perror("socket");
            return (1);
        }
        fd_sockets.push_back(fd_socket);
    }
    return 0;
    
}


int setup_connection_socket(std::vector<int>& fd_sockets, std::vector<int>& listPorts) 
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;  
    // addr.sin_addr.s_addr = inet_addr("127.0.0.2");



    for (size_t i = 0; i < listPorts.size(); ++i) 
    {
        int fd_socket = fd_sockets[i];
        int opt = 1;
        
        
        if (setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0) 
        {
            perror("setsockopt failed");
            close(fd_socket);
            return 1;
        }

        
        addr.sin_port = htons(listPorts[i]);

        
        if (bind(fd_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) 
        {
            perror("binding failed");
            close(fd_socket);
            return 1;
        }

        
        if (listen(fd_socket, 10) < 0) 
        {
            std::cerr << "Failed to listen on port " << listPorts[i] << std::endl;
            close(fd_socket);
            return 1;
        }

        std::cout << "Listening on port: " << listPorts[i] << std::endl;
    }
    return 0;
}


int start_all_servers(std::vector<int>& fd_sockets, std::vector<Server>& Servers, std::vector<Conf>& Configs)
{
    int numServers = 0;
    std::vector<int> listPorts;


    for (std::vector<Conf>::iterator it = Configs.begin(); it != Configs.end(); ++it)
    {
        numServers++;
    }

    std::cout << "numServers: " << numServers << std::endl;
    

    for (int i = 0; i < numServers; i++)
    {
        /**
         * Server start
         */
        Server server(Configs[i]);
        Servers.push_back(server);

        for (int j = 0; j < server.getPorts().size(); j++)
        {
            int port = server.getPorts()[j];
            std::cout << "Port: " << port << std::endl;
            if (std::find(listPorts.begin(), listPorts.end(), port) == listPorts.end())
            {
                listPorts.push_back(port);

            }
        }
        
    }

    for (std::vector<int>::iterator it = listPorts.begin(); it != listPorts.end(); ++it)
    {
        std::cout << "ListPorts : " << *it << std::endl;
    }


    /**
     * @brief Reglages des connexion et communication "Sockets"
     */
    if (socket_start(fd_sockets, listPorts) > 0)
        return 1;

    if (setup_connection_socket(fd_sockets, listPorts) > 0)
        return 1;

    // std::cout << "Return" << std::endl;
    return 0;
}


int findServerIndex(std::string const& request, std::vector<Server>& Servers) 
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

    
    std::cout << "Request Host: " << host << ", Port: " << port << std::endl;

    
    for (size_t i = 0; i < Servers.size(); ++i) 
    {
        std::cout << "Checking Server index " << i << std::endl;
        std::cout << "Server Host: " << Servers[i].getConf().getConfig("host") << ", Ports: ";
        
        
        const std::vector<int>& serverPorts = Servers[i].getPorts();
        for (std::vector<int>::const_iterator it = serverPorts.begin(); it != serverPorts.end(); ++it)
            std::cout << *it << " ";
        std::cout << std::endl;
        
        
        if (Servers[i].getConf().getConfig("host") == host && 
            std::find(serverPorts.begin(), serverPorts.end(), port) != serverPorts.end()) 
        {
            std::cout << "Match found at index " << i << std::endl;
            return i; 
        }
    }

    
    std::cout << "No match found, defaulting to index 0" << std::endl;
    std::cout << Servers[0].getConf().getConfig("host") << std::endl;
    return 0;
}

void type_request_manager(int *fd_client, std::string *req, char *buff, int *reads, Server *server, Epoll *epoll, SessionManager &sessionManager, std::vector<Server>& Servers)
{
    
    std::ofstream outfile("./sessions/fd_client2.txt", std::ios::app);
    if (outfile.is_open())
    {
        outfile << "**************************" << std::endl;
        outfile << "Requête complète : " << std::endl;
        outfile << *req << std::endl;
        outfile << "*************************************" << std::endl;
        outfile.close();
    }
    else
    {
        std::cerr << "Impossible d'ouvrir le fichier fd_client2.txt" << std::endl;
    }

    
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
        if (filePath.find("cgi-bin/") == 0 && server->getCgiStatus()) 
        {
            // if (request.isValidBodySize())
            // {
                Cgi_handler cgiHandler;
                cgiHandler.executeCGI(filePath, request, *fd_client);
            // }
            // else 
            // {
            //     response.setRedirection(413);
            //     response.send(*fd_client);
            // }
        }
        else if (filePath.find("cgi-bin/") == 0 && !server->getCgiStatus())
        {
            response.setRedirection(403);
            response.send(*fd_client);
        }
        else 
        {
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
        std::cerr << "Requête incomplète : en attente de plus de données." << std::endl;
    }
}

void request_and_response_fd_manager(std::vector<int>& fd_sockets, std::vector<Server>& Servers, Conf &conf, SessionManager &sessionManager)
{
    std::map<int, std::string> requestMap;
    Epoll epoll(10);

    for (size_t i = 0; i < fd_sockets.size(); ++i)
    {
        epoll.addFd(fd_sockets[i], EPOLLIN);  
        epoll.makeSocketNonBlocking(fd_sockets[i]);  
    }

    char buff[BUFFER_SIZE];
    int fd_client;
    int reads;
    struct epoll_event event;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;

    while (true) 
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
                    perror("accept");
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

                    
                    if (contentLength == -1 || totalRead >= headerEndPos + 4 + contentLength) 
                    {
                        int serverIndex = findServerIndex(req, Servers);

                        
                        type_request_manager(&fd_client, &req, buff, &reads, &Servers[serverIndex], &epoll, sessionManager, Servers);

                        
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

void get_all_server_conf(std::string const& path, std::vector<Conf>& Configs) 
{
    std::ifstream file(path.c_str());
    if (!file.is_open()) 
    {
        std::cerr << "Unable to open file: " << path << std::endl;
        return;
    }
    
    std::map<int, std::string> map_conf;
    std::string line;
    std::string server_block;
    bool in_server_block = false;
    int server_index = 0;
    
    while (std::getline(file, line)) {
        
        if (line.find("server {") != std::string::npos) 
        {
            in_server_block = true;
            server_block = line + "\n";
        } 
        
        else if (in_server_block && line.find("}") != std::string::npos) 
        {
            server_block += line + "\n";
            map_conf[server_index++] = server_block;  
            in_server_block = false;
            server_block.clear();
        } 
        
        else if (in_server_block) {
            server_block += line + "\n";
        }
    }

    file.close();

    std::ofstream outfile_map("./sessions/server_map.txt");
    if (!outfile_map.is_open())
    {
        std::cerr << "Unable to open output file: sessions/server_map.txt" << std::endl;
        return;
    }

    int index = 0;
    for (std::map<int, std::string>::iterator it = map_conf.begin(); it != map_conf.end(); ++it)
    {
        outfile_map << "map #" << index << ": " << std::endl << it->second << std::endl;
        index++;
    }
    std::cout << "index: " << index << std::endl;


    outfile_map.close();

    index = 0;
    
    for (std::map<int, std::string>::iterator it = map_conf.begin(); it != map_conf.end(); ++it)
    {
        
        std::ofstream temp_file("./config/temp_server_block.conf");
        std::string temp_file_path = "./config/temp_server_block.conf";
        temp_file << it->second;
        temp_file.close();
        
        
        Conf conf(temp_file_path);
        Configs.push_back(conf);
        remove("./config/temp_server_block.conf");
        index++;
    }
    std::cout << "index: " << index << std::endl;

    

    
    std::ofstream outfile("./sessions/server_map_conf.txt");
    if (!outfile.is_open()) {
        std::cerr << "Unable to open output file: sessions/server_map_conf.txt" << std::endl;
        return;
    }

    for (size_t i = 0; i < Configs.size(); ++i) {
        outfile << "Configuration du serveur " << i << " :" << std::endl;
        Configs[i].printConfigs(outfile);
        outfile << std::endl;
    }
    std::cout << "Size Conf: " << Configs.size() << std::endl;
    std::cout << "Host Conf: " << Configs[0].getConfig("host") << std::endl; 
    std::cout << "Host Conf: " << Configs[1].getConfig("host") << std::endl;
    outfile.close();
}

int main(int ac, char **av)
{
    std::string path;
    int fd_socket;
    std::vector<int> fd_sockets;
    int count;
    SessionManager sessionManager;
    std::vector<Conf> Configs;
    std::vector<Server> Servers;

    /**
     * Conditions du path, si NULL, path par defaut
     */
    if (ac >= 2)
        path.assign(av[1]);
    else 
        path = "config/server.conf";

    /**
     * Extraire les informations dans le path
     */
    // Conf conf(path);
    get_all_server_conf(path, Configs);
   
    if (start_all_servers(fd_sockets, Servers, Configs) == 1)
        return 1;
    
    /**
     * @brief Gestion du trafic de requetes et reponses (fd du client et du serveur)
     */
    request_and_response_fd_manager(fd_sockets, Servers, Configs[0], sessionManager);
    for (int i = 0; i < fd_sockets.size() ; ++i)
    {
        close(fd_sockets[i]);
    }
    return (0);
}
