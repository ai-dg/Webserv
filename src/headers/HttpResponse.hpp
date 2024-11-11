#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "../headers/Server.hpp"
#include "../headers/HttpRequest.hpp"
#include "../headers/files.hpp"
#include "../headers/stringUtils.hpp"
#include "../headers/date.hpp"
#include "../headers/includes.hpp"

#define AUTO 200


class HttpResponse
{
    private :
        std::map<std::string, std::string> headers;
        std::string mimeType;
        std::string filePath;
        int statusCode;
        void setMineType(void);
        
    public :
        HttpResponse(const HttpRequest &req);
        ~HttpResponse();
        void setStatusCode(int stat);
        void setRedirection(std::string newPath);
        void setRedirection(int status);
        void setRedirection(std::string newPath, int status);
        void checkRedirection(const HttpRequest &req);
        std::string getHeaders();
        void send(int fd_client);
        void setResourcePath(const HttpRequest &req);
        std::string getFilePath() const;
        void addHeader(const std::string &key, const std::string &value);

};

#endif
