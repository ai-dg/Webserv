#include "../headers/HttpRequest.hpp"
#include "../headers/colors.hpp"

/**
 * @brief Public:
 */
HttpRequest::HttpRequest(std::string req)
{
    std::cout << std::endl << BOLD_YELLOW << req << RESET << std::endl;
    parseRequest(req);
}

HttpRequest::HttpRequest(std::string req, Server *server)
{
    std::cout << std::endl << "--START--" << BOLD_YELLOW << req << RESET << "--END--" << std::endl;
    parseRequest(req);
    this->server = server;
    //this->postbody = getBody();
    //std::cout << "Test map : " << this->headers["Connection"] << std::endl;
}

HttpRequest::HttpRequest(std::string req, std::vector<Server> Servers)
{
    parseRequest(req);
   // headers["Host"];
    std::vector<Server>::iterator it;
    for (it = Servers.begin(); it != Servers.end(); ++it)
    {
        if (it->foundHostName(headers["Host"]))
            server = &(*it);
    }
    //this->server = server;
    //this->postbody = getBody();
    //std::cout << "Test map : " << this->headers["Connection"] << std::endl;
}

HttpRequest::~HttpRequest()
{    
    std::cout << "end req" << std::endl;
}

std::string HttpRequest::getRequestedFile() const
{
    return this->URI;
}

// std::string HttpRequest::getQueryString() const
// {
//     size_t pos = this->URI.find("?");
//     if (pos != std::string::npos) {
//         return this->URI.substr(pos + 1); 
//     }
//     return "";
// }

std::string HttpRequest::getQueryString() const
{
    size_t pos = this->URI.find("?");
    std::cout << "-----------URI: " << this->URI << std::endl;
    if (pos != std::string::npos && pos + 1 < this->URI.size())
    {
        std::string queryString = this->URI.substr(pos + 1);
        std::cout << "------------Extracted Query String: " << queryString << std::endl; 
        return queryString;
    }
    else
    {
        std::cout << "-----------No query string found in URI." << std::endl;
    }
    return "";
}

std::string HttpRequest::getURI() const
{
    return this->URI;
}

std::string HttpRequest::getHeader(std::string key)
{
    return this->headers[key];
}

std::string HttpRequest::getFormatedHeader(std::string key)
{
    return "HTTP_" + upperCaseMe(key) + ": " + this->headers[key];
}

std::map<std::string, std::string> HttpRequest::getHeaders() const
{
    return headers;
}

std::string HttpRequest::getMethod() const
{
    return this->method;
}

std::string HttpRequest::getBody() const
{

    std::cerr << BOLD_WHITE << "METHOD / " << method << RESET << std::endl;
    if (method == "POST") 
        return body;
    else if (method == "GET") 
        return getQueryString();
    else if (method == "DELETE") 
        return "";
    return "";//body;
}

/**
 * @brief Private:
 */

void HttpRequest::setMethod(std::string req)
{
    size_t spacePos = req.find(" ");
    if (spacePos != std::string::npos)
        this->method = req.substr(0, spacePos);
    else
        this->method = "";
}

// void HttpRequest::setHeaders(std::string req)
// {
//     int crlfPos = req.find(CRLF);
//     int start = crlfPos + 1;
//     std::string headers = req.substr(start, std::string::npos);
//     while(crlfPos != std::string::npos)
//     {
//         crlfPos = headers.find(CRLF);
//         std::string line = headers.substr(0, crlfPos);
//         this->addToHeaders(line);
//         headers.erase(0, crlfPos + 2);
//     }
// }

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

// void HttpRequest::setBody(std::string req)
// {
//     this->body = req;
// }

void HttpRequest::setBody(std::string req)
{
    size_t bodyPos = req.find("\r\n\r\n");
    if (bodyPos != std::string::npos)
    {
        this->body = req.substr(bodyPos + 4);
    }
    else
    {
        this->body = "";
    }
}

bool HttpRequest::isValidBodySize() const
{
    size_t size = body.size();
    if (size <= server->getMaxBodySize())
        return true;
    return false;
}



// void HttpRequest::parseRequest(std::string req)
// {
    
//     this->setMethod(req);
    
//     this->setURI(req);
   
//     this->setHeaders(req);
   
//    // std::cout << "method : " << this->method << std::endl;
//     //std::cout << "URI : " << this->URI << std::endl;

//     // Parser la methode - verifier si elle est acceptée par le serveur (voir le parsing du fichier server.conf et stocker ces informations dans un tableau)
    
// }

void HttpRequest::parseRequest(std::string req)
{
    this->setMethod(req);
    this->setURI(req);
    this->setHeaders(req);

    // std::cerr << "------------Method: " << this->method << std::endl;
    // std::cerr << "--------********************req: " << req << std::endl;
    // std::cerr << "--------*************************" << std::endl;
   
    if (this->method == "POST")
    {
        // Extraire le corps de la requête après les en-têtes
        size_t bodyStartPos = req.find("\r\n\r\n");
        if (bodyStartPos != std::string::npos)
        {
            this->body = req.substr(bodyStartPos + 4);
            std::cerr << "-------Parsed Body: " << this->body << std::endl << "-------end parsed body" << std::endl;
        }
    }
}

// void HttpRequest::setURI(std::string req)
// {
//     int backPos = req.find("/");
//     int spacePos = 0;
//     if (backPos != std::string::npos)
//         spacePos = req.find(" ", backPos);
//     if (spacePos != std::string::npos && spacePos != 0)
//          this->URI = req.substr(backPos, spacePos - backPos);
//     else
//         this->URI = "";
//     //this->filePath = "www" + this->URI;
//     //std::cout << "space : " << spacePos << " - / : " << backPos << std::endl;
// }

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

    std::cout << "HttpRequest::setURI" << std::endl << "-----------Extracted URI: " << BLUE << this->URI << RESET << std::endl;
}

// void HttpRequest::addToHeaders(std::string line)
// {
//     int pos = line.find(":");
//     std::string first;
//     std::string second;
//     if (pos != std::string::npos)
//     {
//         first = line.substr(0, pos);
//         second = line.substr(pos + 2, std::string::npos);
//         this->headers[first] = second;
//         //std::cout << "keyval : " << first << " - " << second << std::endl;
//     }
//     else 
//         this->setBody(line);
// }

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
    {
        this->setBody(line);
    }
}