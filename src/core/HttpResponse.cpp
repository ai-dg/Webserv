#include "../headers/HttpResponse.hpp"
#include "../headers/colors.hpp"
#include <unistd.h>
#include <sstream>

// void HttpResponse::setResourcePath(const HttpRequest &req)
// {
//     /// "www/" a modifier en fonction du parsing de configuration du serveur 
//     this->filePath = "www/html" + req.getURI();
// }

HttpResponse::HttpResponse(const HttpRequest &req)
{
    this->setResourcePath(req);
    this-> setMineType();
}

HttpResponse::~HttpResponse()
{

}

void HttpResponse::send(int fd_client)
{
    std::string resFile = getFile(this->filePath);
    if (resFile == FILENOTFOUND)
        this->statusCode = 404;
    else
        this->statusCode = 200;

    std::string res = "HTTP/1.1 " + numberToString(this->statusCode) + " OK\r\n";

    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        res += it->first + ": " + it->second ;
    }
    
    res += "\r";

    std::cout << "---------- res by line ----------" << std::endl;
    std::istringstream ss(res);
    std::string line;
    while (std::getline(ss, line)) 
    {
        std::cout << line << std::endl;
    }
    std::cout << "---------------------------------" << std::endl;

    res += "Content-Type: " + this->mimeType + "; charset=UTF-8\r\n" + 
           "Connection: keep-alive\r\n" + 
           "Content-Length: " + numberToString(resFile.size()) + "\r\n" +
           "Date: " + get_current_date() + "\r\n\r\n" + 
           resFile;

    write(fd_client, res.c_str(), res.size());
    std::ofstream file("./sessions/fd_client.txt"); // Chemin du fichier pour l'écriture
    if (file.is_open()) 
    {
        file << res;
        file.close();
        std::cout << "----------fd_client enregistré dans fd_client.txt---------------" << std::endl;
    } 
    else 
    {
        std::cerr << "Erreur : impossible d'ouvrir le fichier ../sessions/fd_client.txt" << std::endl;
    } 
    std::cout << RED << "\nResponse sent with status: " << this->statusCode << RESET << std::endl;
}


// void HttpResponse::send(int fd_client)
// {
//     std::string resFile = getFile(this->filePath);
//     if (resFile == FILENOTFOUND)
//         this->statusCode = 404;
//     else
//         this->statusCode = 200;
//     std::cout << "status : " << this->statusCode << std::endl;
//     std::string res = "HTTP/1.1 " + numberToString(this->statusCode) + " OK\r\nContent-Type: "+ this->mimeType + "; charset=UTF-8 " + 
//             "\r\nConnection: keep-alive" + 
//             "\r\nContent-Length: " + numberToString(resFile.size()) +
//             "\r\nDate: " + get_current_date() + 
//             "\r\n\r\n" + resFile;   
//     write(fd_client, res.c_str(), res.size());    
//     std::cout << RED << "done" << RESET << std::endl;
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

void HttpResponse::addHeader(const std::string &key, const std::string &value)
{
    headers[key] = value;
    std::cout << "-------------Header added: " << key << " = " << value << std::endl;
    std::cout << "-------------Current headers in response:" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
}

void HttpResponse::setMineType(void)
{
    // protections et verifications a faires/// tests a faire avec netcat et telnet en envoyant des demandes erronées pour les fichiers
    this->mimeType = checkMimeType(this->filePath);
}

