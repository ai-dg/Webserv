#include "../headers/HttpResponse.hpp"
#include "../headers/colors.hpp"
#include <unistd.h>

// void HttpResponse::setResourcePath(const HttpRequest &req)
// {
//     /// "www/" a modifier en fonction du parsing de configuration du serveur 
//     this->filePath = "www/html" + req.getURI();
// }

void HttpResponse::setResourcePath(const HttpRequest &req)
{
    std::string uri = req.getURI();

    if (uri.find("/cgi-bin/") != std::string::npos || uri.find(".py") != std::string::npos || uri.find(".php") != std::string::npos) 
    {
        this->filePath = "cgi-bin" + uri;
    }
    else 
    {
        this->filePath = "www/html" + uri;
    }

    std::cout << "File path set to: " << this->filePath << std::endl;
}

std::string HttpResponse::getFilePath() const
{
    return this->filePath;
}

void HttpResponse::setMineType(void)
{
    // protections et verifications a faires/// tests a faire avec netcat et telnet en envoyant des demandes erronées pour les fichiers
    this->mimeType = checkMimeType(this->filePath);
}

void HttpResponse::send(int fd_client)
{
    std::string resFile = getFile(this->filePath);
    if (resFile == FILENOTFOUND)
        this->statusCode = 404;
    else
        this->statusCode = 200;
    std::cout << "status : " << this->statusCode << std::endl;
    std::string res = "HTTP/1.1 " + numberToString(this->statusCode) + " OK\r\nContent-Type: "+ this->mimeType + "; charset=UTF-8 " + 
            "\r\nConnection: keep-alive" + 
            "\r\nContent-Length: " + numberToString(resFile.size()) +
            "\r\nDate: " + get_current_date() + 
            "\r\n\r\n" + resFile;   
    write(fd_client, res.c_str(), res.size());    
    std::cout << RED << "done" << RESET << std::endl;
}

HttpResponse::HttpResponse(const HttpRequest &req)
{
    this->setResourcePath(req);
    this-> setMineType();
}

HttpResponse::~HttpResponse()
{

}
