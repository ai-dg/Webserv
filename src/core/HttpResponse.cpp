#include "../headers/HttpResponse.hpp"
#include "../headers/colors.hpp"
#include "../headers/Status.hpp"
#include "../headers/defines.hpp"
#include "../headers/index.hpp"
#include "../headers/directories.hpp"
#include <unistd.h>
#include <cstdlib>
#include <sstream>

// void HttpResponse::setResourcePath(const HttpRequest &req)
// {
//     /// "www/" a modifier en fonction du parsing de configuration du serveur 
//     this->filePath = "www/html" + req.getURI();
// }

HttpResponse::HttpResponse(const HttpRequest &req)
{
    setResourcePath(req);
    body = "";
    setMineType();
    Log::output("./sessions/HttpResponse.txt") << "is valid body size : " << req.isValidBodySize() << std::endl;
    if (!req.isValidBodySize())
    {
        setRedirection(413);
    }
}

HttpResponse::~HttpResponse()
{

}

// void HttpResponse::send(int fd_client, int statusCode)
// {

// }

void HttpResponse::send(int fd_client)
{   
    std::string resFile;
    if (body.size() > 0)
        resFile = body;
    else 
        resFile = getFile(this->filePath);
    if (resFile == FILENOTFOUND && statusCode !=301 && statusCode !=302)
        this->statusCode = 404;

    std::string res = "HTTP/1.1 " + numberToString(this->statusCode) + Status::get(statusCode) + CRLF;
    res += getHeaders();
    /*for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        res += it->first + ": " + it->second ;
    }*/
    
   /* res += "\r";*/

    Log::output("./sessions/HttpResponse.txt") << "---------- res by line ----------" << std::endl;
    std::istringstream ss(res);
    std::string line;
    while (std::getline(ss, line)) 
    {
        Log::output("./sessions/HttpResponse.txt") << line << std::endl;
    }
    Log::output("./sessions/HttpResponse.txt") << "---------------------------------" << std::endl;

    res += "Content-Type: " + this->mimeType + "; charset=UTF-8\r\n" + 
           "Connection: keep-alive\r\n" + 
           "Date: " + get_current_date() + CRLF;
    if (statusCode != 301 && statusCode != 302)
    {
        res += "Content-Length: " + numberToString(resFile.size()) + CRLF + CRLF 
         + resFile;
    } 
    else
    {
        res += "\r\n\r\n";
    }

    std::cerr << RED << res << RESET << std::endl;

    write(fd_client, res.c_str(), res.size());
    std::ofstream file("./sessions/fd_client.txt"); // Chemin du fichier pour l'écriture
    if (file.is_open()) 
    {
        file << res;
        file.close();
        //Log::output("./sessions/HttpResponse.txt") << "----------fd_client enregistré dans fd_client.txt---------------" << std::endl;
    } 
    else 
    {
        Log::output("./logs/error.log") << "Erreur : impossible d'ouvrir le fichier ../sessions/fd_client.txt" << std::endl;
    } 
    Log::output("./sessions/HttpResponse.txt") << RED << "\nResponse sent with status: " << this->statusCode << RESET << std::endl;
}


// void HttpResponse::send(int fd_client)
// {
//     std::string resFile = getFile(this->filePath);
//     if (resFile == FILENOTFOUND)
//         this->statusCode = 404;
//     else
//         this->statusCode = 200;
//     Log::output("./sessions/HttpResponse.txt") << "status : " << this->statusCode << std::endl;
//     std::string res = "HTTP/1.1 " + numberToString(this->statusCode) + " OK\r\nContent-Type: "+ this->mimeType + "; charset=UTF-8 " + 
//             "\r\nConnection: keep-alive" + 
//             "\r\nContent-Length: " + numberToString(resFile.size()) +
//             "\r\nDate: " + get_current_date() + 
//             "\r\n\r\n" + resFile;   
//     write(fd_client, res.c_str(), res.size());    
//     Log::output("./sessions/HttpResponse.txt") << RED << "done" << RESET << std::endl;
// }

void HttpResponse::setRedirection(std::string newPath)
{
    filePath = newPath;
}

void HttpResponse::setRedirection(int status)
{
    std::stringstream fp;
    switch (status)
    {
        case 403:
        case 404:
        case 413:
        case 500: fp << "www/error_pages/" << status <<".html";
                    filePath = fp.str(); break;   
        default: break;
    }  
    setStatusCode(status);
    
}

void HttpResponse::setRedirection(std::string newPath, int status)
{
    filePath = newPath;
    setStatusCode(status);
}

void HttpResponse::setStatusCode(int stat)
{
    if (stat == AUTO)
    {
        if (getFile(this->filePath) == FILENOTFOUND)
            statusCode = 404;
        else
            this->statusCode = 200;
    }
    else
        statusCode = stat;
}

void HttpResponse::checkRedirection(const HttpRequest &req)
{
    (void) req;
    //req.server->getConf();
}

std::string HttpResponse::getHeaders()
{
    std::string res = "";
    std::map<std::string, std::string>::iterator it = headers.begin();
    for (; it != headers.end(); ++it)
    {
        res += it->first + ": " + it->second + CRLF;
    }
    return res;
}

void HttpResponse::setResourcePath(const HttpRequest &req)
{
    std::string uri = req.getURI();
    std::cerr << "URI :::: " << uri << std::endl;
    Location *Route = req.getRouteConf(req.getRoute());
    if (!Route)
    {
        setRedirection(403);
        std::cerr << "NO ROUTE FOUND" << std::endl;
        return;
    }
    if (Route->redirection() != "")
    {
        std::cerr << "Redirection : " << Route->getRedirectionPath() << "  -  "  << Route->getRedirectionStatus() << std::endl;
        setRedirection(Route->getRedirectionPath(), Route->getRedirectionStatus());
        addHeader("Location", Route->getRedirectionPath());
        return;
    }

    std::cerr << "this one :::: ???? : " << Route->root() << std::endl;

    if (uri.find("/cgi-bin/") != std::string::npos
        || uri.find(".py") != std::string::npos
        || uri.find(".pl") != std::string::npos
        || uri.find(".sh") != std::string::npos
        || uri.find(".php") != std::string::npos) 
    {
        Route = req.getRouteConf("/cgi-bin/");
        //this->filePath = Route->root() + uri;
         this->filePath = Route->root().substr(1, std::string::npos) + "/" + req.getAskedFile();
    }
    /*else if (uri.find(".jpg") != std::string::npos
        || uri.find(".png") != std::string::npos
        || uri.find(".svg") != std::string::npos)
    {
        this->filePath = uri;
    }*/
    else
    {
        if (uri =="/")
            uri += Route->index(); ///// attention... si plusieurs index.... faire une fonction avec access pour vérifier qu'au moins un des chemins existe
        
        this->filePath = Route->root() + "/" + req.getAskedFile();
    }
    
    if(Route->methods().find(req.getMethod()) == std::string::npos)
    {
        std::cerr << "NO METHOD MATCH" << std::endl;
        setRedirection(403);
        return ;
    }
    if (pathIsDir("./" + Route->root()) && Route->autoindex() == "on")
    {
        std::cerr << RED << "IIIIIIIIIIIIIIIIIIIIIIIIIINNNNNN" <<std::endl;
        setBody(getIndexFile("./" + Route->root() + "/"));
    }

    setStatusCode(AUTO);
    //req.printConf(LOCATION_ROOT);

    std::cerr << "path / : " << this->filePath  << std::endl;
    
    
    //Log::output("./sessions/HttpResponse.txt") << BOLD_GREEN << "File path set to: " << this->filePath << RESET << std::endl;
}

void HttpResponse::setBody(std::string content)
{
    body = content;
}

std::string HttpResponse::getFilePath() const
{
    return this->filePath;
}

void HttpResponse::addHeader(const std::string &key, const std::string &value)
{
    headers[key] = value;
    Log::output("./sessions/HttpResponse.txt") << "-------------Header added: " << key << " = " << value << std::endl;
    Log::output("./sessions/HttpResponse.txt") << "-------------Current headers in response:" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        Log::output("./sessions/HttpResponse.txt") << it->first << ": " << it->second << std::endl;
    }
}

void HttpResponse::setMineType(void)
{
    // protections et verifications a faires/// tests a faire avec netcat et telnet en envoyant des demandes erronées pour les fichiers
    this->mimeType = checkMimeType(this->filePath);
}

