#include <iostream>
#include "../headers/date.hpp"
#include "../headers/format.hpp"
#include "../headers/Server.hpp"

int main(int ac, char **av)
{
    (void) ac;
    (void) av;
    std::ostringstream req;
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
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(fd_socket,(struct sockaddr*) &addr, sizeof(addr));
    listen(fd_socket, 10);
    struct  sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int fd_client = accept(fd_socket, (struct sockaddr *) &client_addr, &client_addr_len);
    char buff[1024];
   /* if (ac == 2)
        server = new Server(av[1]);
    else
        server = new Server();
    (void) server;
    std::cout << get_current_date() << std::endl;*/
    bzero(buff, 1024);
    std::cout << "test" << std::endl;
    int reads = read(fd_client, buff, 1024);
    std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello, World!</h1></body></html>";
    write(fd_client, response.c_str(), response.size());
    while(reads > 0)
    {
        req << buff;
        std::cout << "test2" << std::endl;
        reads = read(fd_client, buff, 1024);
    }
    std::cout << req.str() << std::endl;
    std::cout << "test3" << std::endl;
    close (fd_client);  
    close (fd_socket);
    //delete (server);
    return (0);
}
