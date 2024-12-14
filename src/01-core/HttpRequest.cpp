/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:57:59 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/11 16:47:24 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/HttpRequest.hpp"
#include "../00-headers/02-utils/Log.hpp"
#include "../00-headers/02-utils/stringUtils.hpp"

HttpRequest::HttpRequest(){}


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

    
    this->method = cleanString(this->method);
    
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

std::string cleanMergedData(const std::string& merged) 
{
    size_t end_pos = merged.find_last_not_of('\n');
    if (end_pos != std::string::npos) 
        return merged.substr(0, end_pos + 1);
    return merged;
}

std::string HttpRequest::mergeChunks(std::string data) 
{
    std::string merged;
    std::stringstream stream(data);
    std::string line;

    while (std::getline(stream, line)) 
    {
        if (!line.empty() && line[line.size() - 1] == '\r') 
            line.erase(line.size() - 1);
        size_t chunk_size = 0;
        std::stringstream chunk_size_stream(line);
        chunk_size_stream >> std::hex >> chunk_size;
        if (chunk_size == 0)
            break;
        if (chunk_size_stream.fail()) 
            throw std::runtime_error("Invalid chunk size: " + line);
        std::string chunk_data;
        chunk_data.resize(chunk_size);
        stream.read(&chunk_data[0], chunk_size);
        merged += chunk_data;
        std::getline(stream, line);
    }
    return cleanMergedData(merged);
}

std::string encodingSecretCode(std::string data, const std::string& value) 
{
    if (value.empty()) 
    {
        std::cerr << "Error: Value for encoding is empty. No transformation applied." << std::endl;
        return data;
    }
    char replacementChar = value[0]; 
    for (size_t i = 0; i < data.size(); ++i) 
        data[i] = replacementChar;
    return data;
}

void HttpRequest::setBody(std::string req)
{
    size_t bodyPos = req.find("\r\n\r\n");
    if (bodyPos != std::string::npos)
        body = req.substr(bodyPos + 4);
    else
        body = "";
    if (getHeader("Transfer-Encoding") == "chunked")
        body = mergeChunks(body);
    std::map<std::string, std::string>::iterator it = headers.begin();
    for (; it != headers.end(); ++it)
        if (it->first.find("X-Secret") != std::string::npos)
            body = encodingSecretCode(body, it->second);
}

void HttpRequest::setRoute()
{
    if (URI.empty()) 
    {
        route = "/";
        return;
    }
    size_t firstSlashPos = URI.find_first_of('/');
    size_t lastSlashPos = URI.find_last_of('/');
    size_t lastDotPos = URI.find_last_of('.');
    if ((lastSlashPos == 0 && URI.size() == 1) || (lastSlashPos == 0 && lastDotPos > lastSlashPos && lastDotPos != std::string::npos))
        route = "/"; 
    else 
    {
        if (firstSlashPos == lastSlashPos) 
            route = URI;
        else 
        {
            size_t nextSlashPos = getNextof(URI, 1, '/');
            if (nextSlashPos != std::string::npos) 
            {
                route = URI.substr(0, nextSlashPos + 1);
            } 
            else 
                route = URI;
        }
    } 
    if (lastDotPos == std::string::npos && !URI.empty() && URI[URI.size() - 1] != '/') {
        route += "/";
    }    
    route = removeDuplicateSlashes(route);
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
    
    
    if (this->method == "POST" || this->method == "DELETE" || this->method == "PUT")
    {
        setBody(req);       
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
}

/**
 * @brief Copelin form
 */
HttpRequest::HttpRequest(std::string req)
{
    parseRequest(req);
}

HttpRequest::HttpRequest(std::string req, std::vector<Server *> Servers)
{
    parseRequest(req);
    std::vector<Server *>::iterator it;
    for (it = Servers.begin(); it != Servers.end(); ++it)
    {
        if ((*it)->foundHostName(headers["Host"]))
            server = (*it);
    }
}

HttpRequest::HttpRequest(std::string req, Server *server)
{
    this->server = server;
    parseRequest(req);
}

HttpRequest::HttpRequest(HttpRequest const& src) : headers(src.headers), method(src.method), host(src.host), URI(src.URI), route(src.route), askedFile(src.askedFile), body(src.body), server(src.server)
{
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
    return *this;
}

HttpRequest::~HttpRequest()
{    
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
    if (pos != std::string::npos && pos + 1 < this->URI.size())
    {
        std::string queryString = this->URI.substr(pos + 1);
        return queryString;
    }
    else
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

    if (method == "POST" || method == "DELETE" || method == "PUT") 
    {
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
        std::cerr << line << std::endl;
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
    if (server->getRoute(extension) != NULL)
    {
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
    if (route)
    {
        if (URI == "/" && route->index().find(".php") != std::string::npos)
        {
            return false;
        }
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
    std::cerr << server->getConf()->getConfig(config);
}
