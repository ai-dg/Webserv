/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:02 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/16 10:29:09 by dagudelo         ###   ########.fr       */
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

HttpResponse::HttpResponse()
{

}

void HttpResponse::setMineType(void)
{
    this->mimeType = checkMimeType(this->filePath);
}

/**
 * @brief Copelin form
 */
HttpResponse::HttpResponse(const HttpRequest &req)
{
    this->req = new HttpRequest(req);
    removeDuplicateSlashes(this->filePath);
    sendBody = true;
    body = "";
}

HttpResponse::HttpResponse(const HttpResponse &src) : headers(src.headers), mimeType(src.mimeType), filePath(src.filePath), body(src.body), statusCode(src.statusCode)
{  
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
    return *this;
}

HttpResponse::~HttpResponse()
{
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
        case 400:
        case 403:
        case 404:
        case 405:
        case 406:
        case 413:
        case 505:
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
    if (sub.find_last_of("/") != std::string::npos)
        sub = sub.substr(0, sub.find_last_of("/"));
    if (sub.find(".") != std::string::npos)
        sub = "";
    return sub + "/";
    
}

void HttpResponse::setResourcePath(const HttpRequest &req)
{
    std::string uri = req.getURI();
    std::string extension = getExtension(uri);
    std::string route = req.getRoute();
    Location *Route = req.getRouteConf(route);
    std::string addToRoute = addSub(route, uri);
    size_t sizeMaxInLocation = 100000 * 1024;
    if (Route)
        sizeMaxInLocation = Route->max_body_size();
    size_t bodySize = 0;
    bodySize = req.getBody().size() + 1.024;
    if (bodySize >= sizeMaxInLocation)
    {
        setRedirection(413);
        return;
    }
    if (!Route)
    {
        this->filePath = "/" + req.getAskedFile();
        setRedirection(404);
        return;
    }
    std::string routed = Route->root() + addSub(route,uri);
    if (Route && Route->extensions() != "" && Route->extensions().find(req.getAskedFile().substr(req.getAskedFile().find("."), std::string::npos)) == std::string::npos)
    {
        setRedirection(403);
        return;
    }
    if (Route && Route->redirection() != "")
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
        }
        else
        {
            if (req.getHeader("User-Agent") == "Go-http-client/1.1")
                setStatusCode(204, NO_BODY);
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
        || uri.find(".cgi") != std::string::npos
        || uri.find(".pl") != std::string::npos
        || uri.find(".sh") != std::string::npos
        || uri.find(".php") != std::string::npos)
    {
        Route = req.getRouteConf("/cgi-bin/");
        if (Route)
            this->filePath = Route->root().substr(1, std::string::npos) + req.getAskedFile();
        else
        {
            setRedirection(500);
            return;

        }
    }
    else if (uri == "/" && Route && Route->index().find(".php") != std::string::npos)
    {
        Route = req.getRouteConf("/");
        if (Route)
            this->filePath = Route->root().substr(1, std::string::npos) + req.getAskedFile();
        else
        {
            setRedirection(500);
            return;
        }
    }
    else
    {
        if (uri =="/")
        {
            if (Route && Route->findIndex() == "")
            {
                setRedirection(403);
                return;
            }
            else
                this->filePath = Route->root() + "/" + Route->findIndex();
        }
        else
        {
            if(req.getAskedFile().find(".") == std::string::npos)
                this->filePath = routed + (Route->findIndex());
            else
                this->filePath = routed  + req.getAskedFile();
        }
    }
    if(Route && Route->methods().find(req.getMethod()) == std::string::npos)
    {
        setRedirection(405);
        addHeader("Allow", Route->methods());
        return ;
    }
    if (pathIsDir("./" + routed) && Route && Route->autoindex() == "on" && req.getAskedFile().size() == 0)
        setBody(getIndexFile("./" + routed + "/"));
    else if (pathIsDir("./" + routed) && Route && Route->autoindex() == "on" && req.getAskedFile().size() > 0)
    {
        addHeader("Content-Disposition", "attachment; filename=\"" + req.getAskedFile() + "\"");
        this->filePath = routed + "/" + req.getAskedFile();
    }
    filePath = removeDuplicateSlashes(this->filePath);
    setStatusCode(AUTO);
}

void HttpResponse::setStatusCode(int stat)
{
    std::string file = getFile(this->filePath);
    if (stat == 413)
        file = getFile("./www/error_pages/413.html");
   
    if (stat == AUTO)
    {
        if (file == FILENOTFOUND && req->getRouteConf(req->getRoute())->findIndex() != "")
            statusCode = 404;
        else
            this->statusCode = 200;
    }
    else
        statusCode = stat;
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
}

void HttpResponse::checkRedirection(const HttpRequest &req)
{
    (void) req;
}

int HttpResponse::put(const HttpRequest &req)
{
    std::string path = req.getRouteConf(req.getRoute())->root();
    std::string file = req.getAskedFile();
    filePath = "." + path + file; 
    std::ofstream outfile(filePath.c_str());
    if (!outfile)
        return -1;
    outfile << req.getBody();
    outfile.close();
    return 1;
}


bool HttpResponse::isAllowedMethod(Location *Route, HttpRequest req) const
{

    return Route && (Route->methods().find(req.getMethod()) != std::string::npos);
}

void HttpResponse::send(int fd_client)
{   
    std::string resFile;
    std::string uri = req->getURI();
    std::string extension = getExtension(uri);    
  
    if (body.size() > 0)
        resFile = body;
    else
        resFile = getFile(this->filePath);

    if (req->getRouteConf(req->getRoute()) && extension == "" && resFile == FILENOTFOUND)
        resFile = PATH_FOUND;

    if (extension == "" && resFile == FILENOTFOUND && !req->getRouteConf(req->getRoute()))
       resFile = PATH_NOT_FOUND;

    if (resFile == FILENOTFOUND && statusCode != 301 && statusCode !=302 && sendBody)
    {
        this->statusCode = 404;
        resFile = getFile("./www/error_pages/404.html");
    }
    if (resFile == PATH_NOT_FOUND)
    {
        this->statusCode = 404;
        resFile = getFile("./www/error_pages/404.html");
    }
    std::string res = "HTTP/1.1 " + numberToString(this->statusCode) + Status::get(statusCode) + CRLF;
    res += getHeaders();
    setMineType();
    if (this->mimeType != "")
        res += "Content-Type: " + this->mimeType + "; charset=UTF-8\r\n";
    res += "Connection: keep-alive\r\n";
    res += "Date: " + get_current_date() + CRLF;
    if (sendBody)
    {
        res += "Content-Length: " + numberToString(resFile.size()) + CRLF + CRLF 
         + resFile;
    } 
    else
        res += CRLF;
    write(fd_client, res.c_str(), res.size());
    Log::print_final_log("Response sent with status: ", this->statusCode, "FD:", fd_client);
}
