#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <string>
#include "stringUtils.hpp"

class Cgi_handler
{
    protected:
        std::string scriptPath;
        std::string queryString;
        int fd_client;
        std::string getExeContext(std::string file);

    public:
        Cgi_handler();
        ~Cgi_handler();
        void executeCGIWithoutFork(const std::string& scriptPath, const std::string& queryString, int fd_client);
        void executeCGI(std::string const& scriptPath, std::string const& queryString, std::string const& method, int fd_client);

};



#endif