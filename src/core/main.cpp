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
    int fd_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_socket == -1)
    {
        perror("socket");
        return (1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
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
    bzero(buff, 2048);
    std::cout << "test" << std::endl;
    int reads = 1;
    while(reads)
    {
        req += buff;
        reads = read(fd_client, buff, 2048);
        std::cout << "reads / " << reads << std::endl;
        std::cout << "test3" << std::endl;
        std::cout << req << std::endl;
        std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello, World!</h1></body></html>"+getCrlf();
        write(fd_client, response.c_str(), response.size());
    }
    close (fd_client);  
    close (fd_socket);
    //delete (server);
    return (0);
}
