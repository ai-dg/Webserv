/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:57:59 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/02 16:33:21 by dagudelo         ###   ########.fr       */
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



// std::string HttpRequest::mergeChunks(std::string const& data) {
//     std::string merged;
//     std::istringstream stream(data);
//     std::string line;

//     while (std::getline(stream, line)) {
//         // Supprimer le '\r' à la fin de chaque ligne (s'il existe)
//         if (!line.empty() && line[line.size() - 1] == '\r') {
//             line.erase(line.size() - 1); // Supprimer le dernier caractère
//         }

//         // Convertir la taille du chunk (en hexadécimal) à un entier
//         size_t chunkSize = 0;
//         std::stringstream chunkSizeStream(line);
//         chunkSizeStream >> std::hex >> chunkSize;

//         if (chunkSize == 0) {
//             break; // Fin des chunks
//         }

//         // Lire les données du chunk en fonction de `chunkSize`
//         std::string chunk(chunkSize, '\0');
//         stream.read(&chunk[0], chunkSize);
//         merged += chunk;

//         // Vérifier qu'il reste au moins deux caractères `\r\n` à ignorer
//         if (stream.peek() == '\r') {
//             stream.get(); // Ignorer '\r'
//         }
//         if (stream.peek() == '\n') {
//             stream.get(); // Ignorer '\n'
//         }
//     }

//     return merged;
// }

std::string HttpRequest::mergeChunks(std::string const& data) {
    std::string merged;
    std::string chunk = data;

    while (!chunk.empty()) {
        
        size_t crlf_pos = chunk.find("\r\n");
        if (crlf_pos == std::string::npos) {
            std::cerr << "Error: Missing CRLF in chunk header." << std::endl;
            break;
        }

        
        std::string chunk_size_str = chunk.substr(0, crlf_pos);
        size_t chunk_size = 0;
        std::stringstream chunk_size_stream(chunk_size_str);
        chunk_size_stream >> std::hex >> chunk_size;

        if (chunk_size_stream.fail()) {
            std::cerr << "Error: Invalid chunk size format: '" << chunk_size_str << "'" << std::endl;
            break;
        }

        if (chunk_size == 0) {
            std::cerr << "End of chunks detected (chunk size 0)." << std::endl;
            break;
        }

        
        chunk.erase(0, crlf_pos + 2);

        
        if (chunk.size() < chunk_size) {
            std::cerr << "Error: Chunk size exceeds remaining data size." << std::endl;
            std::cerr << "Chunk size: " << chunk_size << ", Remaining size: " << chunk.size() << std::endl;
            
            merged += chunk.substr(0, chunk.size());
            break;
        }

        
        merged += chunk.substr(0, chunk_size);

        
        chunk.erase(0, chunk_size);

        
        if (chunk.size() >= 2 && chunk.substr(0, 2) == "\r\n") {
            chunk.erase(0, 2);
        } else if (!chunk.empty()) {
            std::cerr << "Warning: Missing CRLF after chunk data. Remaining data: " << chunk << std::endl;
            break;
        }
    }

    std::cerr << "Final merged size: " << merged.size() << std::endl;
    return merged;
}




void HttpRequest::setBody(std::string req)
{
    std::cerr << YELLOW << "REQ SIZE BODY "  << req.size() << RESET << std::endl;
    Log::output("./sessions/test.txt") << req << std::endl;
    size_t bodyPos = req.find("\r\n\r\n");
    if (bodyPos != std::string::npos)
        body = req.substr(bodyPos + 4);
    else
        body = "";
    if (getHeader("Transfer-Encoding") == "chunked")
        body = mergeChunks(body);
    
    std::cerr << YELLOW << "SIZE BODY "  << req.size() << RESET << std::endl;
    Log::output("./sessions/test2.txt") << body << std::endl;
     Log::output("./sessions/HttpRequest.txt") << "-------Parsed Body: " << body << std::endl << "-------end parsed body" << std::endl;
    std::cerr << YELLOW << "Yes it's chunked" << RESET << std::endl;
}

// void HttpRequest::setRoute()
// {   
//     size_t firstSlashPos = URI.find_first_of('/'); 
//     size_t lastSlashPos = URI.find_last_of('/');
//     size_t lastDotPos = URI.find_last_of('.');

//    /* if (lastDotPos != std::string::npos && lastDotPos > lastSlashPos)
//     {*/
//     if (lastSlashPos == 0 && URI.size() == 1)
//         route = "/"; 
//     else
//     {
//         if (firstSlashPos == lastSlashPos)
//             route = URI;
//         else
//             route = URI.substr(0, getNextof(URI,1,'/') + 1); 
//     }
        
//     if (lastDotPos == std::string::npos && URI[URI.size()-1] != '/')
//         route += "/";
//     route = removeDuplicateSlashes(route);
//     /*}*/
//     /*else
//     { 
//         if (URI[URI.size() - 1] != '/')
//             route = URI + "/"; 
//         else
//             route = URI; 
//     }*/
//     // std::cerr << BOLD_VIOLET << "URI : " << URI << " ------------- extracted route : "<< route << std::endl;
// }

void HttpRequest::setRoute()
{
    
    if (URI.empty()) {
        route = "/";
        return;
    }

    size_t firstSlashPos = URI.find_first_of('/');
    size_t lastSlashPos = URI.find_last_of('/');
    size_t lastDotPos = URI.find_last_of('.');
    if ((lastSlashPos == 0 && URI.size() == 1) || (lastSlashPos == 0 && lastDotPos > lastSlashPos && lastDotPos != std::string::npos))
    {
        route = "/"; 
    }
    else {
        if (firstSlashPos == lastSlashPos) {
            
            route = URI;
        } else {
            
            size_t nextSlashPos = getNextof(URI, 1, '/');
            if (nextSlashPos != std::string::npos) {
                route = URI.substr(0, nextSlashPos + 1);
            } else {
                route = URI; 
            }
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
    
    if (this->method == "POST" || this->method == "DELETE")
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
    }/*
    else
        this->setBody(line);*/
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
        // std::cerr << "getBody : " << body << std::endl;
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
    // Log::output("./sessions/HttpRequest.txt") << "Body max size: " << server->getMaxBodySize() << std::endl;
    // Log::output("./sessions/HttpRequest.txt") << "Body size: " << body.size() << std::endl;
    // Log::output("./sessions/HttpRequest.txt") << "Body: " << body << std::endl;
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
