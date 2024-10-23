#include "../headers/HttpResponse.hpp"


void HttpResponse::setResourcePath(const HttpRequest &req)
{
    /// "www/" a modifier en fonction du parsing de configuration du serveur 
    this->filePath = "www" + req.getURI();
}
void HttpResponse::setMineType(void)
{
    // protections et verifications a faires/// tests a faire avec netcat et telnet en envoyant des demandes erronées pour les fichiers
    this->mimeType = checkMimeType(this->filePath);
}

void HttpResponse::send(int fd_client)
{
    std::string res = "HTTP/1.1 200 OK\r\nContent-Type: "+ this->mimeType + "; charset=UTF-8 " + 
            CRLF + "Connection: keep-alive" + 
            CRLF + "Content-Length: XXX" +
            CRLF + CRLF + getFile(this->filePath);
    int size = res.size() - 3;
    size += numberToString(size).size();
    res = replaceBy(res, "XXX", numberToString(size));
    std::cout << "test res : " << res << std::endl;
    write(fd_client, res.c_str(), res.size());    
}

HttpResponse::HttpResponse(const HttpRequest &req)
{
    this->setResourcePath(req);
    this-> setMineType();
}

HttpResponse::~HttpResponse()
{

}
