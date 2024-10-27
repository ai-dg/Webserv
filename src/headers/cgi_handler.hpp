#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <string>
#include <vector>
#include "stringUtils.hpp"


class Cgi_handler
{
    protected:
        std::string scriptPath;
        std::string queryString;
        int fd_client;
        std::vector<char *> environment;
        std::string getExeContext(std::string file);
        void addToEnvironment(std::string env);
        void addToEnvironment(const char * env);
 
    public:
        Cgi_handler();
        ~Cgi_handler();
        void executeCGI(std::string const& scriptPath, std::string const& queryString, std::string const& method, int fd_client);

};



#endif