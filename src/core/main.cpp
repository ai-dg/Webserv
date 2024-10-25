#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
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
#include "../headers/SessionManager.hpp"
#include "../headers/Cookies.hpp"

#define BUFFER_SIZE 2048

int socket_start(std::vector<int>& fd_sockets, Server *server)
{
    /**
    *    int fd_socket = socket(AF_INET, SOCK_STREAM, 0);
    *    Creation d'un socket permettant la connextion
    *    l'option AF_INET permet de choisir le protocole de connexion Protocoles Internet IPv4
    *    l'option SOCK_STREAM permet de choisir le type de connexion TCP man : (
    *    SOCK_STREAM Support de dialogue garantissant l'intégrité, fournissant un flux de données binaires, 
    *    et intégrant un mécanisme pour les transmissions de données hors-bande. )
    */

    int count  = server->getNumPorts();

    fd_sockets.clear();
    for (int i = 0; i < count; i++)
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

int setup_connection_socket(std::vector<int>& fd_sockets, Server *server)
{
    int count;
    int* ports = server->getPort(count);

    /**
     * struct sockaddr_in afin de configurer le socket
     */
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = server->getAddr();

    /**
     * Redemarre le serveur en cas de crash pour pouvoir reutiliser le port
     */

    for (int i = 0; i < count; i++)
    {
        int fd_socket = fd_sockets[i];

        int opt = 1;
        setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));


        addr.sin_port = htons(ports[i]);

        if (bind(fd_socket,(struct sockaddr*) &addr, sizeof(addr)) < 0)
        {
            perror("binding failed");
            close(fd_socket);
            return (1);
        }

        /**
         * Mets en ecoute les connections du socket et 
         * un nombre maximal de connections simultannees, il refuse les autres
         */
        if (listen(fd_socket, 10) < 0)
        {
            std::cout << "fail listening socket on port " << ports[i] << std::endl;
            close(fd_socket);
            return(1);        
        }
        std::cout << "Listening on port: " << ports[i] << std::endl;
    }

    return 0;
}

void type_request_manager(int *fd_client, std::string *req, char *buff, int *reads, Server *server, Epoll *epoll, SessionManager &sessionManager)
{
    (void)sessionManager;
    *req += std::string(buff, *reads);
    if ((*req).find("\r\n\r\n") != std::string::npos) 
    {
        HttpRequest request(*req, server);

        // std::string cookieHeader = request.getHeader("Cookie");
        // Cookies cookies(cookieHeader);

        // std::string sessionId = cookies.getCookie("sessionId");
        // if (!sessionManager.sessionExist(sessionId))
        // {
        //     sessionId = sessionManager.createSessions();
        //     cookies.setCookie("sessionId", sessionId);
        // }

        // std::map<std::string, std::string>& sessionData = sessionManager.getSession(sessionId);

        HttpResponse response(request);
        response.setResourcePath(request);

        // response.addHeader("Set-Cookie", cookies.getSetCookieHeader());

        std::string filePath = response.getFilePath();  
        if (filePath.find("cgi-bin/") == 0) 
        {
            Cgi_handler cgiHandler;
            std::cout << "Executing script..." << std::endl;
            if (request.getMethod() == "POST") 
            {
                std::string postBody = request.getBody();
                cgiHandler.executeCGI(filePath, postBody, "POST", *fd_client);
            } 
            else if (request.getMethod() == "GET") 
                cgiHandler.executeCGI(filePath, request.getQueryString(), "GET", *fd_client);
            else if (request.getMethod() == "DELETE") 
                cgiHandler.executeCGI(filePath, "", "DELETE", *fd_client);
        } 
        else 
            response.send(*fd_client);
        *req = "";
        if (request.getHeader("Connection") != "keep-alive") 
        {
            close(*fd_client);
            epoll->removeFd(*fd_client);
        }
    }
}

void request_and_response_fd_manager(std::vector<int>& fd_sockets, Server *server, Conf &conf, SessionManager &sessionManager)
{
    /**
     * @brief
     * Creation de la classe Epoll, une API pour gerer les evenements d'entree/sortie sur plusieurs FD,
     * methode plus effiface en comparaison de poll() et select(), pas besoin d'examiner chaque descripteur
     * de chaque appel.
     */
    Epoll epoll(10);
    for (size_t i = 0; i < fd_sockets.size(); ++i)
    {
        epoll.addFd(fd_sockets[i], EPOLLIN);  
        epoll.makeSocketNonBlocking(fd_sockets[i]);  
    }
    char buff[BUFFER_SIZE];
    std::string req;
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
            /**
             * @brief Detecte s'il y a une connexion entrant dans le serveur
             */

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
            /**
             * @brief Une fois detecte il va traiter la demande du client
             */
            else if (event.events & EPOLLIN) 
            {
                
                fd_client = event.data.fd;
                bzero(buff, BUFFER_SIZE);
                reads = read(fd_client, buff, BUFFER_SIZE);
                if (reads == 0) 
                {
                    close(fd_client);
                    epoll.removeFd(fd_client);
                } 
                else if (reads > 0) 
                {
                    type_request_manager(&fd_client, &req, buff, &reads, server, &epoll, sessionManager);
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
}

int main(int ac, char **av)
{
    std::string path;
    int fd_socket;
    std::vector<int> fd_sockets;
    int count;
    SessionManager sessionManager;

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
   
    /**
     * Server start
     */
    Server server(conf);
    count = server.getNumPorts();
    /**
     * @brief Reglages des connexion et communication "Sockets"
     */
    if (socket_start(fd_sockets, &server) > 0)
        return 1;

    if (setup_connection_socket(fd_sockets, &server) > 0)
        return 1;
    
    /**
     * @brief Gestion du trafic de requetes et reponses (fd du client et du serveur)
     */
    request_and_response_fd_manager(fd_sockets, &server, conf, sessionManager);
    for (size_t i = 0; i < count; i++)
    {
        close(fd_sockets[i]);
    }
    return (0);
}
