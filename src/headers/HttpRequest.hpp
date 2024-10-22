#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <iostream>
#include <map>

class HttpRequest
{
    private :
        std::map<std::string, std::string> headers;
        std::string method;
        std::string host;
        int port;
        
    public :

        HttpRequest(std::string req);
        ~HttpRequest();

};

#endif
