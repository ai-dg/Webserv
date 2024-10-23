#include "../headers/HttpRequest.hpp"


HttpRequest::HttpRequest(std::string req)
{
    std::cout << req << std::endl;
}

void HttpRequest::setMethod(std::string req)
{
    int spacePos = req.find(" ");
    if (spacePos != std::string::npos)
         this->method = req.substr(0, spacePos);
    else
        this->method = "";
}

void HttpRequest::setURI(std::string req)
{
    int backPos = req.find("/");
    int spacePos = 0;
    if (backPos != std::string::npos)
        spacePos = req.find(" ", backPos);
    if (spacePos != std::string::npos && spacePos != 0)
         this->URI = req.substr(backPos, spacePos - backPos);
    else
        this->URI = "";
    //this->filePath = "www" + this->URI;
    //std::cout << "space : " << spacePos << " - / : " << backPos << std::endl;
}

void HttpRequest::addToHeaders(std::string line)
{
    int pos = line.find(":");
    std::string first;
    std::string second;
    if (pos != std::string::npos)
    {
        first = line.substr(0, pos);
        second = line.substr(pos + 2, std::string::npos);
        this->headers[first] = second;
        std::cout << "keyval : " << first << " - " << second << std::endl;
    }
    else 
        this->setBody(line);
}

std::string HttpRequest::getHeader(std::string key)
{
    return this->headers[key];
}

void HttpRequest::setHeaders(std::string req)
{
    int crlfPos = req.find(CRLF);
    int start = crlfPos + 1;
    std::string headers = req.substr(start, std::string::npos);
    while(crlfPos != std::string::npos)
    {
        crlfPos = headers.find(CRLF);
        std::string line = headers.substr(0, crlfPos);
        this->addToHeaders(line);
        headers.erase(0, crlfPos + 2);
    }
}

std::string HttpRequest::getURI() const
{
    return this->URI;
}


void HttpRequest::setBody(std::string req)
{
    this->body = req;
}


void HttpRequest::parseRequest(std::string req)
{
    std::cout << "1 "  << std::endl;
    this->setMethod(req);
      std::cout << "2 " << std::endl;
    this->setURI(req);
      std::cout << "3 " << std::endl;
    this->setHeaders(req);
      std::cout << "4 " << std::endl;
    std::cout << "method : " << this->method << std::endl;
    std::cout << "URI : " << this->URI << std::endl;

    // Parser la methode - verifier si elle est acceptée par le serveur (voir le parsing du fichier server.conf et stocker ces informations dans un tableau)
    
}

HttpRequest::HttpRequest(std::string req, Server *server)
{
    std::cout << req << std::endl;
    parseRequest(req);
    this->server = server;
    std::cout << "Test map : " << this->headers["Connection"] << std::endl;
}

HttpRequest::~HttpRequest()
{    
    std::cout << "end req" << std::endl;
}