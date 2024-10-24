#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <string>

class Cgi_handler
{
    protected:
        std::string scriptPath;
        std::string queryString;
        int fd_client;

    public:
        Cgi_handler();
        ~Cgi_handler();
        void executeCGI(std::string const& scriptPath, std::string const& queryString, int fd_client);

};



#endif