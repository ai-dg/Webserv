/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:02 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/29 18:54:38 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/HttpResponse.hpp"
#include "../00-headers/01-core/Status.hpp"
#include "../00-headers/01-core/index.hpp"
#include "../00-headers/02-utils/directories.hpp"
#include "../00-headers/02-utils/Log.hpp"
#include "../00-headers/02-utils/stringUtils.hpp"

/**
 * @brief Private setters
 */
void HttpResponse::setMineType(void)
{
    // protections et verifications a faires/// tests a faire avec netcat et telnet en envoyant des demandes erronées pour les fichiers
    this->mimeType = checkMimeType(this->filePath);
}

/**
 * @brief Copelin form
 */
HttpResponse::HttpResponse(const HttpRequest &req)
{
    setResourcePath(req);
    removeDuplicateSlashes(this->filePath);
    body = "";
    setMineType();
    Log::output("./sessions/HttpResponse.txt") << "is valid body size : " << req.isValidBodySize() << std::endl;
    if (!req.isValidBodySize())
        setRedirection(413);
    Log::output("./sessions/HttpResponse.txt") << "HttpResponse object class created" << std::endl;
}

HttpResponse::HttpResponse(const HttpResponse &src) : headers(src.headers), mimeType(src.mimeType), filePath(src.filePath), body(src.body), statusCode(src.statusCode)
{
    Log::output("./sessions/HttpResponse.txt") << "HttpResponse object class copied" << std::endl;    
}

HttpResponse &HttpResponse::operator=(const HttpResponse &src)
{
    if (this == &src)
        return *this;
    headers = src.headers;
    mimeType = src.mimeType;
    filePath = src.filePath;
    body = src.body;
    statusCode = src.statusCode;
    Log::output("./sessions/HttpResponse.txt") << "HttpResponse object class assigned" << std::endl;
    return *this;
}

HttpResponse::~HttpResponse()
{
    Log::output("./sessions/HttpResponse.txt") << "HttpResponse object class destroyed" << std::endl;
    Log::cleanup();
}

/**
 * @brief Setters
 */
void HttpResponse::setRedirection(std::string newPath, int status)
{
    filePath = newPath;
    setStatusCode(status);
}

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
        case 405:
        case 413:
        case 500: fp << "www/error_pages/" << status <<".html";
                    filePath = fp.str(); break;   
        default: break;
    }  
    setStatusCode(status);    
}

void HttpResponse::setResourcePath(const HttpRequest &req)
{
    std::string uri = req.getURI();
    std::string route = req.getRoute();
    Location *Route = req.getRouteConf(route);
    if (!Route)
    {
        this->filePath = "/" + req.getAskedFile();
        setRedirection(403);
        return;
    }
    if (Route->extensions() != "" && Route->extensions().find(req.getAskedFile().substr(req.getAskedFile().find("."), std::string::npos)) == std::string::npos)
    {
        setRedirection(403);
        return;
    }
    if (Route->redirection() != "")
    {
        setRedirection(Route->getRedirectionPath(), Route->getRedirectionStatus());
        addHeader("Location", Route->getRedirectionPath());
        return;
    }
    std::string extension = getExtension(uri);
    if (req.hasFileSpecialRoute(filePath))
    {
        Location *altRoute = req.getRouteConf(getExtension(filePath));
        this->filePath = altRoute->root().substr(1, std::string::npos) + req.getAskedFile();
        return;
    }
    if (uri.find("/cgi-bin/") != std::string::npos
        || uri.find(".py") != std::string::npos
        || uri.find(".pl") != std::string::npos
        || uri.find(".sh") != std::string::npos
        || uri.find(".php") != std::string::npos || 
        (uri == "/" && Route->index().find(".php") != std::string::npos))
    {
        Route = req.getRouteConf("/cgi-bin/");
        this->filePath = Route->root().substr(1, std::string::npos) + req.getAskedFile();
        std::cerr << Route->root().substr(1, std::string::npos) << "   " << req.getAskedFile() << std::endl;
        std::cerr << "1 -- setResoursePath -- " << this->filePath << std::endl;
    }
    else
    {
        if (uri =="/")
        {
            if (Route->findIndex() == "")
            {
                setRedirection(403);
                return;
            }
            else
                this->filePath = Route->root() + "/" + Route->findIndex();

        }
        else
            this->filePath = Route->root() + "/" + req.getAskedFile();
    }
    this->method = req.getMethod();
    if(Route->methods().find(req.getMethod()) == std::string::npos)
    {
        std::cerr << "NO METHOD MATCH" << std::endl;
        setRedirection(405);
        addHeader("Allow", Route->methods());
        return ;
    }
    if (pathIsDir("./" + Route->root()) && Route->autoindex() == "on" && req.getAskedFile().size() == 0)
        setBody(getIndexFile("./" + Route->root() + "/"));
    else if (pathIsDir("./" + Route->root()) && Route->autoindex() == "on" && req.getAskedFile().size() > 0)
    {
        addHeader("Content-Disposition", "attachment; filename=\"" + req.getAskedFile() + "\"");
        this->filePath = Route->root() + "/" + req.getAskedFile();
    }
    setStatusCode(AUTO);    
    filePath = removeDuplicateSlashes(this->filePath);
    Log::output("./sessions/HttpResponse.txt") << BOLD_GREEN << "File path set to: " << this->filePath << RESET << std::endl;
}

void HttpResponse::setStatusCode(int stat)
{
    if (stat == AUTO)
    {
        std::cout << "Body: " << body << std::endl;
        if (getFile(this->filePath) == FILENOTFOUND)
            statusCode = 404;
        else
            this->statusCode = 200;

        if (body.size() == 0 && method == "POST")
            statusCode = 405;
    }
    else
        statusCode = stat;
}

void HttpResponse::setBody(std::string content)
{
    body = content;
}

/**
 * @brief Getters
 */
std::string HttpResponse::getHeaders()
{
    std::string res = "";
    std::map<std::string, std::string>::iterator it = headers.begin();
    for (; it != headers.end(); ++it)
        res += it->first + ": " + it->second + CRLF;
    return res;
}

std::string HttpResponse::getFilePath() const
{
    return this->filePath;
}

int HttpResponse::getStatusCode() const
{
    return this->statusCode;
}

/**
 * @brief Public methods
 */
void HttpResponse::addHeader(const std::string &key, const std::string &value)
{
    headers[key] = value;
    Log::output("./sessions/HttpResponse.txt") << "-------------Header added: " << key << " = " << value << std::endl;
    Log::output("./sessions/HttpResponse.txt") << "-------------Current headers in response:" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        Log::output("./sessions/HttpResponse.txt") << it->first << ": " << it->second << std::endl;
    }
}

void HttpResponse::checkRedirection(const HttpRequest &req)
{
    (void) req;
}

int HttpResponse::put(const HttpRequest &req)
{
    (void) req;
    std::ofstream outfile(&filePath.c_str()[1]);
    if (!outfile)
    {
        std::cerr << "fail creating file";
        return -1;
    }
    std::cout << BLUE << req.getBody() << RESET << std::endl;
    outfile << req.getBody();
    outfile.close();
    return 1;
}

void HttpResponse::send(int fd_client)
{   
    std::string resFile;
    if (body.size() > 0)
        resFile = body;
    else 
        resFile = getFile(this->filePath);
    if (resFile == FILENOTFOUND && statusCode !=301 && statusCode !=302)
    {
        this->statusCode = 404;
        resFile = getFile("./www/error_pages/404.html");
    }
    std::string res = "HTTP/1.1 " + numberToString(this->statusCode) + Status::get(statusCode) + CRLF;
    res += getHeaders();
    Log::output("./sessions/HttpResponse.txt") << "---------- res by line ----------" << std::endl;
    std::istringstream ss(res);
    std::string line;
    while (std::getline(ss, line)) 
        Log::output("./sessions/HttpResponse.txt") << line << std::endl;
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
        res += "\r\n\r\n";

    write(fd_client, res.c_str(), res.size());
    std::ofstream file("./sessions/fd_client.txt");
    if (file.is_open()) 
    {
        file << res;
        file.close();
    } 
    else 
        Log::output("./logs/error.log") << "Erreur : impossible d'ouvrir le fichier ../sessions/fd_client.txt" << std::endl;
    Log::output("./sessions/HttpResponse.txt") << RED << "\nResponse sent with status: " << this->statusCode << RESET << std::endl;
}
