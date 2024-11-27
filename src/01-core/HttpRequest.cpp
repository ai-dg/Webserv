/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:57:59 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/27 18:57:58 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/HttpRequest.hpp"
#include "../00-headers/02-utils/Log.hpp"
#include "../00-headers/02-utils/stringUtils.hpp"

/**
 * @brief Private setters
 */
void HttpRequest::setMethod(std::string req)
{
    size_t spacePos = req.find(" ");
    if (spacePos != std::string::npos)
        this->method = req.substr(0, spacePos);
    else
        this->method = "";
}

void HttpRequest::setHeaders(std::string req)
{
    size_t crlfPos = req.find(CRLF); 

    size_t start = crlfPos + 2;
    std::string headersPart = req.substr(start, req.find("\r\n\r\n"));
    while ((crlfPos = headersPart.find(CRLF)) != std::string::npos)
    {       
        std::string line = headersPart.substr(0, crlfPos);
        this->addToHeaders(line);
        headersPart.erase(0, crlfPos + 2);
    }
}

void HttpRequest::setBody(std::string req)
{
    size_t bodyPos = req.find("\r\n\r\n");
    if (bodyPos != std::string::npos)
        this->body = req.substr(bodyPos + 4);
    else
        this->body = "";
}

void HttpRequest::setRoute()
{    
    size_t lastSlashPos = URI.find_last_of('/');
    size_t lastDotPos = URI.find_last_of('.');

    if (lastDotPos != std::string::npos && lastDotPos > lastSlashPos)
    {
        if (lastSlashPos == 0)
            route = "/"; 
        else
            route = URI.substr(0, lastSlashPos + 1); 
    }
    else
    { 
        if (URI[URI.size() - 1] != '/')
            route = URI + "/"; 
        else
            route = URI; 
    }
}

void HttpRequest::setAskedFile()
{
    Location *Rte = NULL;
    if (URI.size() == 1 && URI == "/")
    {
        Rte = server->getRoute(URI);
        if (!Rte)
            return ;
        askedFile = Rte->index();
    }
    else 
        askedFile = URI.substr(URI.find_last_of("/") + 1, URI.size() - URI.find_last_of("/") - 1 );
}

void HttpRequest::setURI(std::string req)
{
    size_t methodEndPos = req.find(" ");
    if (methodEndPos == std::string::npos)
    {
        this->URI = "";
        return;
    }
    size_t uriStartPos = methodEndPos + 1;
    size_t uriEndPos = req.find(" ", uriStartPos);
    if (uriEndPos == std::string::npos)
    {
        this->URI = "";
        return;
    }
    this->URI = req.substr(uriStartPos, uriEndPos - uriStartPos);
    Log::output("./sessions/HttpRequest.txt") << "HttpRequest::setURI" << std::endl << "-----------Extracted URI: " << BLUE << this->URI << RESET << std::endl;
}

/**
 * @brief Private parsers
 */
void HttpRequest::parseRequest(std::string req)
{
    setMethod(req);
    setURI(req);
    setRoute();
    setAskedFile();
    setHeaders(req);
    setBody(req);
    if (this->method == "POST" || this->method == "DELETE")
    {
        size_t bodyStartPos = req.find("\r\n\r\n");
        if (bodyStartPos != std::string::npos)
        {
            this->body = req.substr(bodyStartPos + 4);
            Log::output("./sessions/HttpRequest.txt") << "-------Parsed Body: " << this->body << std::endl << "-------end parsed body" << std::endl;
        }
    }
}

void HttpRequest::addToHeaders(std::string line)
{
    size_t pos = line.find(":");
    if (pos != std::string::npos)
    {
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 2);
        this->headers[key] = value;
    }
    else
        this->setBody(line);
}

/**
 * @brief Copelin form
 */
HttpRequest::HttpRequest(std::string req)
{
    Log::output("./sessions/HttpRequest.txt") << "HttpRequest class object created" << std::endl;
    Log::output("./sessions/HttpRequest.txt") << std::endl << BOLD_YELLOW << req << RESET << std::endl;
    parseRequest(req);
}

HttpRequest::HttpRequest(std::string req, std::vector<Server *> Servers)
{
    Log::output("./sessions/HttpRequest.txt") << "HttpRequest class object created" << std::endl;
    parseRequest(req);
    std::vector<Server *>::iterator it;
    for (it = Servers.begin(); it != Servers.end(); ++it)
    {
        if ((*it)->foundHostName(headers["Host"]))
            server = (*it);
    }
    Log::output("./sessions/HttpRequest.txt") << "Test map : " << this->headers["Connection"] << std::endl;
}

HttpRequest::HttpRequest(std::string req, Server *server)
{
    Log::output("./sessions/HttpRequest.txt") << "HttpRequest class object created" << std::endl;
    this->server = server;
    Log::output("./sessions/HttpRequest.txt") << std::endl << "--START--" << BOLD_YELLOW << req << RESET << "--END--" << std::endl;
    parseRequest(req);
    Log::output("./sessions/HttpRequest.txt") << "Test map : " << this->headers["Connection"] << std::endl;
}

HttpRequest::HttpRequest(HttpRequest const& src) : headers(src.headers), method(src.method), host(src.host), URI(src.URI), route(src.route), askedFile(src.askedFile), body(src.body), server(src.server)
{
    Log::output("./sessions/HttpRequest.txt") << "HttpRequest class object copied" << std::endl;
}

HttpRequest& HttpRequest::operator=(HttpRequest const& src)
{
    if (this != &src)
    {
        this->headers = src.headers;
        this->method = src.method;
        this->host = src.host;
        this->URI = src.URI;
        this->route = src.route;
        this->askedFile = src.askedFile;
        this->body = src.body;
        this->server = src.server;
    }
    Log::output("./sessions/HttpRequest.txt") << "HttpRequest class object assigned" << std::endl;
    return *this;
}

HttpRequest::~HttpRequest()
{    
    Log::output("./sessions/HttpRequest.txt") << "HttpRequest class object destroyed" << std::endl;
    Log::cleanup();
}


/**
 * @brief Getters
 */
std::string HttpRequest::getRoute() const
{
    return route;
}

std::string HttpRequest::getRequestedFile() const
{
    return this->URI;
}

std::string HttpRequest::getQueryString() const
{
    size_t pos = this->URI.find("?");
    Log::output("./sessions/HttpRequest.txt") << "-----------URI: " << this->URI << std::endl;
    if (pos != std::string::npos && pos + 1 < this->URI.size())
    {
        std::string queryString = this->URI.substr(pos + 1);
        Log::output("./sessions/HttpRequest.txt") << "------------Extracted Query String: " << queryString << std::endl; 
        return queryString;
    }
    else
        Log::output("./sessions/HttpRequest.txt") << "-----------No query string found in URI." << std::endl;
    return "";
}

std::string HttpRequest::getURI() const
{
    return this->URI;
}

std::string HttpRequest::getAskedFile() const
{
    return askedFile;
}

std::string HttpRequest::getConf(std::string key) const
{
    return this->server->getConf()->getConfig(key);
}

std::string HttpRequest::getHeader(std::string key) const
{
    if (headers.find(key) != headers.end())
        return headers.find(key)->second;
    return "";
}

std::string HttpRequest::getFormatedHeader(std::string key)
{
    return "HTTP_" + upperCaseMe(key) + ": " + this->headers[key];
}

std::string HttpRequest::getMethod() const
{
    return this->method;
}

std::string HttpRequest::getBody() const
{

    Log::output("./sessions/HttpRequest.txt") << BOLD_WHITE << "METHOD / " << method << RESET << std::endl;
    if (method == "POST" || method == "DELETE" || method == "PUT") 
    {
        std::cerr << "getBody : " << body << std::endl;
        return body;
    }
    else if (method == "GET") 
        return getQueryString();
    return "";
}

void HttpRequest::getHostByName() const
{
    std::ifstream hosts("/etc/hosts");
    std::string line;
    while (std::getline(hosts, line))
    {
        std::cout << line << std::endl;
    }
    hosts.close();
}

Location *HttpRequest::getRouteConf(std::string const & route) const
{
    return server->getRoute(route);
}

std::map<std::string, std::string> HttpRequest::getHeaders() const
{
    return headers;
}

/**
 * @brief Validators
 */

bool HttpRequest::hasFileSpecialRoute(std::string filePath) const
{
    std::string extension = getExtension(filePath);
   // std::cerr << BLUE << filePath << "-----------" << extension << RESET << std::endl;
    if (server->getRoute(extension) != NULL)
    {
       // std::cerr << " yeah you did it baby !!!!!! " << std::endl;
        return true;
    }
    return false;
}

bool HttpRequest::isScript() const
{
    if (hasFileSpecialRoute(getAskedFile()))
        return true;
    return false;
}

bool HttpRequest::isStatic() const
{
    std::vector<std::string> extensions;
        extensions.push_back(".js");
        extensions.push_back(".html");
        extensions.push_back(".htm");
        extensions.push_back(".css");
        extensions.push_back(".jpg");
        extensions.push_back(".jpeg");
        extensions.push_back(".gif");
        extensions.push_back(".png");
        extensions.push_back(".ico");
        extensions.push_back(".pdf");
        extensions.push_back(".ttf");
    std::vector<std::string>::iterator it;
    for (it = extensions.begin(); it != extensions.end(); ++it)
    {
        if (URI.find(*it) != std::string::npos)
            return true;
    }
     Location *route = getRouteConf("/");
    if (URI.find(".php") != std::string::npos)
    {
        return false;
    }
    if (URI == "/" && route->index().find(".php") != std::string::npos)
    {
        return false;
    }
    else
        return true;
    return false;
}

bool HttpRequest::isValidBodySize() const
{
    size_t size = body.size();
    if (size <= server->getMaxBodySize())
        return true;
    return false;
}

/**
 * @brief Printers
 */

void HttpRequest::printConf(std::string config) const
{
    std::cout << server->getConf()->getConfig(config);
}
