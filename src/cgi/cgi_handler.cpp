#include "../headers/cgi_handler.hpp"
#include <iostream>
#include <unistd.h>     
#include <sys/types.h>  
#include <sys/wait.h>   
#include <cstdlib>      
#include <cstring>      
#include <fcntl.h>      
#include <cstdio>

Cgi_handler::Cgi_handler()
{
    std::cout << "CGI Handler created" << std::endl;

}

Cgi_handler::~Cgi_handler()
{
    std::cout << "CGI Handler destroyed" << std::endl;

}

void Cgi_handler::executeCGI(std::string const& scriptPath, std::string const& queryString, int fd_client)
{
   
}
