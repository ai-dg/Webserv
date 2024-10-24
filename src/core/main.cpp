#include <iostream>
#include "../headers/date.hpp"
#include "../headers/format.hpp"
#include "../headers/Server.hpp"
#include "../headers/files.hpp"
#include "../headers/parser.hpp"
#include "../headers/HttpRequest.hpp"
#include "../headers/HttpResponse.hpp"


int main(int ac, char **av)
{
    std::string req;
    std::string path;
    Server *server;
    if (ac >= 2)
        path.assign(av[1]);
    else 
        path = "config/server.conf";

    server = new Server(path);
    server->getHostipv4();
    setServer(path, server);
   /**
    *  int fd_socket = socket(AF_INET, SOCK_STREAM, 0);
    * Creation d'un socket permettant la connextion
    *    l'option AF_INET permet de choisir le protocole de connexion Protocoles Internet IPv4
    *    l'option SOCK_STREAM permet de choisir le type de connexion TCP man : (
    *    SOCK_STREAM Support de dialogue garantissant l'intégrité, fournissant un flux de données binaires, 
    *   et intégrant un mécanisme pour les transmissions de données hors-bande. )
    */
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
    addr.sin_port = htons(server->getPort());
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    // redemarre le serveur en cas de crash pour pouvoir reutiliser le port
    int opt = 1;
    setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    if (bind(fd_socket,(struct sockaddr*) &addr, sizeof(addr)) < 0)
        perror("binding failed");
    if (listen(fd_socket, 10) < 0)
    {
        std::cout << "fail listening socket" << std::endl;
        return(1);        
    }  
    struct  sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int fd_client = accept(fd_socket, (struct sockaddr *) &client_addr, &client_addr_len);
    if (fd_client < 0)
    {
        std::cout << "fail opening fd" << std::endl;
        return(1);
    }
    char buff[2048];
    int reads;
    bzero(buff, 2048);
    while(true)
    {
        reads = read(fd_client, buff, 2048);
        if (reads)
        {
            req += buff;
            bzero(buff, 2048);
            if (reads == 0 || reads < 2048)
            {
                HttpRequest request(req, server);
                HttpResponse response(request);
                response.send(fd_client);
                req = "";
                std::cout << BLUE << request.getHeader("Connection") << RESET << std::endl;
                if(request.getHeader("Connection") != "keep-alive")
                {
                    std::cout << "end : " << request.getHeader("Connection") << std::endl;
                    close (fd_client);
                    break;
                } 
            }
        }
        //std::cout << "req : " << req << std::endl;
   }
    close (fd_socket);
    delete (server);
    return (0);
}
