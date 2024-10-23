#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "../headers/Server.hpp"
#include "../headers/HttpRequest.hpp"
#include "../headers/files.hpp"


class HttpResponse
{
    private :
        std::map<std::string, std::string> headers;
        std::string mimeType;
        std::string filePath;
        void setResourcePath(const HttpRequest &req);
        void setMineType(void);
        
    public :

        void send(int fd_client);
        HttpResponse(const HttpRequest &req);
        ~HttpResponse();

};

#endif
