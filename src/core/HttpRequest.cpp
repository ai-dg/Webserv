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
    
}

void HttpRequest::setHeaders(std::string req)
{
    int crlfPos = req.find(CRLF);
    int start = crlfPos + 1;
    std::string headers = req.substr(start, std::string::npos);
    while(crlfPos != std::string::npos)
    {
        crlfPos = req.find(CRLF);
        std::string line = headers.substr(start, crlfPos - 2);
        this->addToHeaders(line);
        start = crlfPos + 2;
    }
    std::cout << "headers : " << std::endl << headers << std::endl;
}

std::string HttpRequest::getURI() const
{
    return this->URI;
}


void HttpRequest::setBody(std::string req)
{

}


void HttpRequest::parseRequest(std::string req)
{
    this->setMethod(req);
    this->setURI(req);
    this->setHeaders(req);
    this->setBody(req);
    std::cout << "method : " << this->method << std::endl;
    std::cout << "URI : " << this->URI << std::endl;

    // Parser la methode - verifier si elle est acceptée par le serveur (voir le parsing du fichier server.conf et stocker ces informations dans un tableau)
    
}

HttpRequest::HttpRequest(std::string req, Server *server)
{
    std::cout << req << std::endl;
    parseRequest(req);
    this->server = server;
}

HttpRequest::~HttpRequest()
{    
    std::cout << "end req" << std::endl;
}