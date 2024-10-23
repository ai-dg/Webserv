#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "../headers/Server.hpp"


class HttpRequest
{
    private :
        std::map<std::string, std::string> headers;
        std::string method;
        std::string host;
        std::string URI;
        int port;
        Server *server;
        void setMethod(std::string req);
        void setHeaders(std::string req);
        void setBody(std::string req);
        void parseRequest(std::string req);
        void setURI(std::string req);
        void addToHeaders(std::string line);
        
    public :

        HttpRequest(std::string req);
        std::string getURI() const;
        HttpRequest(std::string req, Server *server);
        ~HttpRequest();

};

#endif
