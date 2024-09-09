#include <iostream>
#include "../headers/date.hpp"
#include "../headers/format.hpp"
#include "../headers/Server.hpp"

int main(int ac, char **av)
{
    (void) ac;
    (void) av;
    std::string req;
   // Server *server = NULL;
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
    addr.sin_port = htons(8080);
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
   /* if (ac == 2)
        server = new Server(av[1]);
    else
        server = new Server();
    (void) server;
    std::cout << get_current_date() << std::endl;*/
    int reads = 1;
    while(reads > 0)
    {
        req += buff;
        bzero(buff, 2048);
        reads = read(fd_client, buff, 2048);
        std::cout << "reads / " << reads << std::endl;
        std::cout << "test3" << std::endl;
        std::cout << req << std::endl;
        if (reads == 0 || reads < 2048)
        {
            std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<html><body><h1>Hello, Diego !!! on a un début de serveur 😀😀😀😀 !!!!<br> Mais tout reste à faire !!!</h1></body></html>\r\n";
            write(fd_client, response.c_str(), response.size());
            close (fd_client);  
            break;
        }
   }
    close (fd_socket);
    //delete (server);
    return (0);
}
