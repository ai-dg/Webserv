#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "../headers/Server.hpp"
#include "../headers/format.hpp"


class HttpRequest
{
    private :
        std::map<std::string, std::string> headers;
        std::string method;
        //std::string postbody;
        std::string host;
        std::string URI;
        std::string body;
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
        HttpRequest(std::string req, std::vector<Server> Servers);
        HttpRequest(std::string req, Server *server);
        ~HttpRequest();

        bool isValidBodySize() const;
        std::string getRequestedFile() const;
        std::string getQueryString() const;
        void getHostByName() const;
        std::string getURI() const;
        void printConf(std::string) const;
        std::string getConf(std::string key) const;
        std::string getHeader(std::string key) const;
        std::string getFormatedHeader(std::string key);
        std::map<std::string, std::string> getHeaders() const;
        std::string getMethod() const;
        std::string getBody() const; 

};

#endif
