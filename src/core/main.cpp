#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include "../headers/date.hpp"
#include "../headers/format.hpp"
#include "../headers/Server.hpp"
#include "../headers/files.hpp"
#include "../headers/parser.hpp"
#include "../headers/HttpRequest.hpp"
#include "../headers/HttpResponse.hpp"
#include "../headers/Conf.hpp"
#include "../headers/cgi_handler.hpp"
#include "../headers/Epoll.hpp"
#include "../headers/Log.hpp"
#include "../headers/ipTools.hpp"

#define BUFFER_SIZE 2048

int main(int ac, char **av)
{
    std::string req;
    std::string path;

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

    Conf conf(path);
    conf.getValuesFromPath();
    //conf.printConfigs();
    conf.checkAndSetDefaultValues();
    conf.printConfigs();

    /**
     * Server start
     */

    Server server(conf);
    setServer(path, &server);

    int fd_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_socket == -1)
    {
        perror("socket");
        return (1);
    }

    /**
     * struct sockaddr_in afin de configurer le socket
     */
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(server.getPort());
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //// remplacer INADDR_LOOPBACK par l'adresse determinee dans conf

    /**
     * Redemarre le serveur en cas de crash pour pouvoir reutiliser le port
     */
    int opt = 1;
    setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));

    if (bind(fd_socket,(struct sockaddr*) &addr, sizeof(addr)) < 0)
        perror("binding failed");


    if (listen(fd_socket, 10) < 0)
    {
        std::cout << "fail listening socket" << std::endl;
        return(1);        
    }

    /**
     * @brief
     * Creation de la classe Epoll, une API pour gerer les evenements d'entree/sortie sur plusieurs FD,
     * methode plus effiface en comparaison de poll() et select(), pas besoin d'examiner chaque descripteur
     * de chaque appel.
     */
    Epoll epoll(10);  
    epoll.addFd(fd_socket, EPOLLIN);  
    epoll.makeSocketNonBlocking(fd_socket);  
    char buff[BUFFER_SIZE];

    while (true) 
    {
        int eventCount = epoll.wait(-1);
        for (int i = 0; i < eventCount; ++i) 
        {
            struct epoll_event event = epoll.getEvent(i);

            if (event.data.fd == fd_socket) 
            {
                
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int fd_client = accept(fd_socket, (struct sockaddr*)&client_addr, &client_addr_len);
                Log::access(get_current_date() + " : Ip " + std::string(inet_ntoa(client_addr.sin_addr)));
                if (fd_client == -1) 
                {
                    Log::error(get_current_date() + " connection failed");
                    perror("accept");
                    continue;
                }
                epoll.makeSocketNonBlocking(fd_client);
                epoll.addFd(fd_client, EPOLLIN | EPOLLET);
            } 
            else if (event.events & EPOLLIN) 
            {
                
                int fd_client = event.data.fd;
                bzero(buff, BUFFER_SIZE);
                int reads = read(fd_client, buff, BUFFER_SIZE);
                if (reads == 0) 
                {
                    close(fd_client);
                    epoll.removeFd(fd_client);
                } 
                else if (reads > 0) 
                {
                    req += std::string(buff, reads);

                    
                    if (req.find("\r\n\r\n") != std::string::npos) 
                    {
                        HttpRequest request(req, &server);
                        HttpResponse response(request);
                        
                        response.setResourcePath(request);
                        std::string filePath = response.getFilePath();  

                        if (filePath.find("cgi-bin/") == 0) 
                        {                            
                            Cgi_handler cgiHandler;
                            std::cout << "Executing script..." << std::endl;
                            if (request.getMethod() == "POST") 
                            {                                
                                std::string postBody = request.getBody();
                                cgiHandler.executeCGI(filePath, postBody, "POST", fd_client);
                            } 
                            else if (request.getMethod() == "GET") 
                            {                                
                                cgiHandler.executeCGI(filePath, request.getQueryString(), "GET", fd_client);
                            } 
                            else if (request.getMethod() == "DELETE") 
                            {                                
                                cgiHandler.executeCGI(filePath, "", "DELETE", fd_client);
                            }
                        } 
                        else 
                        {   
                            response.send(fd_client);
                        }

                        req = "";  

                        if (request.getHeader("Connection") != "keep-alive") 
                        {
                            close(fd_client);
                            epoll.removeFd(fd_client);
                        }
                    }
                    Epoll::purgeTimeOutFds(conf, epoll.getFd());
                } 
                else 
                {
                    
                    perror("read");
                    close(fd_client);
                    epoll.removeFd(fd_client);
                }
            }
        }
    }
    close (fd_socket);
    return (0);
}
