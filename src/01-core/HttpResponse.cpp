/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:02 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/04 21:37:45 by dagudelo         ###   ########.fr       */
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
    //setResourcePath(req);
    this->req = new HttpRequest(req);
    removeDuplicateSlashes(this->filePath);
    sendBody = true;
    body = "";
    setMineType();
    // Log::output("./sessions/HttpResponse.txt") << "is valid body size : " << req.isValidBodySize() << std::endl;
    
    
    // if (!req.isValidBodySize())
    //     setRedirection(413);
    // Log::output("./sessions/HttpResponse.txt") << "HttpResponse object class created" << std::endl;
}

HttpResponse::HttpResponse(const HttpResponse &src) : headers(src.headers), mimeType(src.mimeType), filePath(src.filePath), body(src.body), statusCode(src.statusCode)
{
    // Log::output("./sessions/HttpResponse.txt") << "HttpResponse object class copied" << std::endl;    
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
    // Log::output("./sessions/HttpResponse.txt") << "HttpResponse object class assigned" << std::endl;
    return *this;
}

HttpResponse::~HttpResponse()
{
    // Log::output("./sessions/HttpResponse.txt") << "HttpResponse object class destroyed" << std::endl;
    delete req;
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
        case 406:
        case 413:
        case 500: fp << "www/error_pages/" << status <<".html";
                    filePath = fp.str(); break;   
        default: break;
    }  
    setStatusCode(status);    
}

std::string HttpResponse::addSub(std::string route, std::string uri)
{
    std::string sub = "";

    if (uri.find(route) != std::string::npos && uri.find(route) == 0)
        sub = uri.substr(route.size(), std::string::npos);
    // std::cerr << "add sub :::::::::::::::::::::::::::: " << sub << std::endl;
    if (sub.find_last_of("/") != std::string::npos)
        sub = sub.substr(0, sub.find_last_of("/"));
    if (sub.find(".") != std::string::npos)
        sub = "";
    return sub + "/";
    
}


////////////////////// new fonctions isValidUri(),bool hasExtension(std::string file);
void HttpResponse::setResourcePath(const HttpRequest &req)
{
    std::string uri = req.getURI();
    std::string extension = getExtension(uri);
    std::string route = req.getRoute();
    Location *Route = req.getRouteConf(route);
    std::string addToRoute = addSub(route, uri);
    size_t sizeMaxInLocation = Route->max_body_size();
    size_t bodySize = req.getBody().size() + 1.024;
    // std::cerr << BOLD_WHITE << "Uri debug " << uri << RESET << std::endl;
    // std::cerr << BOLD_WHITE << "Extension debug " << extension << RESET << std::endl;
    // std::cerr << BOLD_WHITE << "SET_RESOURCE_PATH _ filePath debug " << filePath << RESET << std::endl;
    // std::cerr << BOLD_WHITE << "SET_RESOURCE_PATH _ uri debug " << uri << RESET << std::endl;
    // std::cerr << BOLD_WHITE << "SET_RESOURCE_PATH _ route " << route << "   " << Route->root() << RESET << std::endl;
    // std::cerr << BOLD_WHITE << "SET_RESOURCE_PATH _ route.index() " << route << "   " << Route->findIndex() << RESET << std::endl;
    // std::cerr << BOLD_WHITE << "SizeMax of body : " << sizeMaxInLocation << RESET << std::endl;
    // std::cerr << BOLD_WHITE << "Size of body : " << bodySize << RESET << std::endl;
    
    

    if (bodySize >= sizeMaxInLocation)
    {
        setRedirection(413);
        return;
    }
    
    if (!Route)
    {
        this->filePath = "/" + req.getAskedFile();
        // std::cerr << BOLD_RED << "NO ROUUUUUUUUTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << RESET << std::endl;
        setRedirection(404);
        return;
    }
    std::string routed = Route->root() + addSub(route,uri);
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
    if (req.hasFileSpecialRoute(extension))
    {
        Location *altRoute = req.getRouteConf(getExtension(extension));
        if (isAllowedMethod(altRoute, req))
        {
            this->filePath = altRoute->root().substr(1, std::string::npos) + req.getAskedFile();
            // std::cerr << "2 -- setResoursePath -- " << this->filePath << std::endl;            
        }
        else
        {
            if (req.getHeader("User-Agent") == "Go-http-client/1.1")
            {
                setStatusCode(204,NO_BODY); //// pffffff
                // std::cerr << "2 . 405 - "<< req.getHeader("User-Agent") << std::endl;
            }
            else
            {
                addHeader("Allow", altRoute->methods());
                setRedirection(405);                
            }
        }
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
        // std::cerr << Route->root().substr(1, std::string::npos) << "   " << req.getAskedFile() << std::endl;
        // std::cerr << "1 -- setResoursePath -- " << this->filePath << std::endl;
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
            {
                this->filePath = Route->root() + "/" + Route->findIndex();
            //   std::cerr << "4 -- setResoursePath -- " << this->filePath << std::endl;   
            }

        }
        else
        {
           //////// + addToRoute ///////
            if(req.getAskedFile().find(".") == std::string::npos)
            {
                this->filePath = routed + (Route->findIndex()); //// findindex ???????
                // std::cerr << "3 --- final : this->filePath : " <<filePath << std::endl;
            }
            else
                this->filePath = routed  + req.getAskedFile();
            // std::cerr << "3 -- setResoursePath -- " << this->filePath  << std::endl;   
            //   std::cerr << get_current_date() << std::endl;
        }
    }
    
    if(Route->methods().find(req.getMethod()) == std::string::npos)
    {
        // std::cerr << "NO METHOD MATCH" << std::endl;
        setRedirection(405);
        addHeader("Allow", Route->methods());
        return ;
    }
    //routed instead of Route->root()
    if (pathIsDir("./" + routed) && Route->autoindex() == "on" && req.getAskedFile().size() == 0)
        setBody(getIndexFile("./" + routed + "/"));
    else if (pathIsDir("./" + routed) && Route->autoindex() == "on" && req.getAskedFile().size() > 0)
    {
        addHeader("Content-Disposition", "attachment; filename=\"" + req.getAskedFile() + "\"");
        this->filePath = routed + "/" + req.getAskedFile();
    }
    filePath = removeDuplicateSlashes(this->filePath);
    setStatusCode(AUTO);    
    // Log::output("./sessions/HttpResponse.txt") << BOLD_GREEN << "File path set to: " << this->filePath << RESET << std::endl;
}

void HttpResponse::setStatusCode(int stat)
{
    std::string file = getFile(this->filePath);
   
    if (stat == AUTO)
    {
        if (file == FILENOTFOUND && req->getRouteConf(req->getRoute())->findIndex() != "")
            statusCode = 404;
        else
            this->statusCode = 200;
    }
    else
        statusCode = stat;
    // std::cerr << "setStatusCode(1) : " << this->filePath << file << "  ->  statusCode :" << statusCode <<std::endl;
}

void HttpResponse::setStatusCode(int stat, int body_status)
{
    if (body_status == NO_BODY)
        sendBody = false;
    std::string file = getFile(this->filePath);
    if (stat == AUTO)
    {
        if (file == FILENOTFOUND && req->getRouteConf(req->getRoute())->findIndex() != "")
            statusCode = 404;
        else
            this->statusCode = 200;
    }
    else
        statusCode = stat;
    // std::cerr << "setStatusCode(2) : " << this->filePath << file << "  ->  statusCode :" << statusCode <<std::endl;
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

/**
 * @brief Public methods
 */
void HttpResponse::addHeader(const std::string &key, const std::string &value)
{
    headers[key] = value;
    // Log::output("./sessions/HttpResponse.txt") << "-------------Header added: " << key << " = " << value << std::endl;
    // Log::output("./sessions/HttpResponse.txt") << "-------------Current headers in response:" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        // Log::output("./sessions/HttpResponse.txt") << it->first << ": " << it->second << std::endl;
    }
}

void HttpResponse::checkRedirection(const HttpRequest &req)
{
    (void) req;
}

int HttpResponse::put(const HttpRequest &req)
{
    //// rajouter la verification du maxbody de la route... renvoyer -1 et set payloadtoo large ou autre
    (void) req;
    std::ofstream outfile(&filePath.c_str()[1]);
    if (!outfile)
    {
        // std::cerr << "fail creating file";
        return -1;
    }
    // std::cerr << BLUE << req.getBody() << RESET << std::endl;
    outfile << req.getBody();
    outfile.close();
    return 1;
}


bool HttpResponse::isAllowedMethod(Location *Route, HttpRequest req) const
{
    return Route->methods().find(req.getMethod()) != std::string::npos;
}

void HttpResponse::send(int fd_client)
{   
    // std::cerr << "SEND _ filepath debug" << filePath << std::endl;
    std::string resFile;
    if (body.size() > 0)
        resFile = body;
    else 
        resFile = getFile(this->filePath);
        
    if (resFile == FILENOTFOUND && statusCode !=301 && statusCode !=302 && sendBody && !req->getRouteConf(req->getRoute()))
    {
        // std::cerr << RED << "404 NOT FOUND" << RESET << std::endl;
        this->statusCode = 404;
        resFile = getFile("./www/error_pages/404.html");
    }
    std::string res = "HTTP/1.1 " + numberToString(this->statusCode) + Status::get(statusCode) + CRLF;
    res += getHeaders();
    // Log::output("./sessions/HttpResponse.txt") << "---------- res by line ----------" << std::endl;
    std::istringstream ss(res);
    std::string line;
    while (std::getline(ss, line)) 
        // Log::output("./sessions/HttpResponse.txt") << line << std::endl;
    // Log::output("./sessions/HttpResponse.txt") << "---------------------------------" << std::endl;
    if (sendBody)
        res += "Content-Type: " + this->mimeType + "; charset=UTF-8\r\n";           
    res += "Connection: keep-alive\r\n";
    res += "Date: " + get_current_date() + CRLF;
    if (statusCode != 301 && statusCode != 302 && sendBody)
    {
        res += "Content-Length: " + numberToString(resFile.size()) + CRLF + CRLF 
         + resFile;
    } 
    else
        res += CRLF;
    //std::cerr << RED << "&" << res << "&" << RESET <<std::endl;
    write(fd_client, res.c_str(), res.size());

    // Log::output("./sessions/fd_client.txt") << res << std::endl;
    // std::ofstream file("./sessions/fd_client.txt");
    // if (file.is_open()) 
    // {
    //     file << res;
    //     file.close();
    // } 
    // else 
    //     // Log::output("./logs/error.log") << "Erreur : impossible d'ouvrir le fichier ../sessions/fd_client.txt" << std::endl;
    // Log::output("./sessions/HttpResponse.txt") << RED << "\nResponse sent with status: " << this->statusCode << RESET << std::endl;
}
