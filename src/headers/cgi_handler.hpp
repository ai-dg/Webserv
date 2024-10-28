#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <string>
#include <vector>
#include "stringUtils.hpp"
#include "HttpRequest.hpp"


class Cgi_handler
{
    protected:
        std::string scriptPath;
        std::string queryString;
        int fd_client;
        std::vector<char *> environment;
        std::string getExeContext(std::string file);
        void setEnvironment(HttpRequest req);
        void debugEnvironment();
        void addToEnvironment(std::string env);
        void addToEnvironment(const char * env);
 
    public:
        Cgi_handler();
        ~Cgi_handler();
        void executeCGI(std::string const& scriptPath, HttpRequest req, int fd_client);

};



#endif