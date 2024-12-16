/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RarManager.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:55:04 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/16 10:39:35 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/RarManager.hpp"
#include "../00-headers/01-core/HttpRequest.hpp"
#include "../00-headers/01-core/RequestResponseManager.hpp"
#include "../00-headers/01-core/HttpResponse.hpp"
#include "../00-headers/01-core/Cookies.hpp"
#include "../00-headers/01-core/SignalHandler.hpp"
#include "../00-headers/02-utils/signals.hpp"
#include "../00-headers/02-utils/Log.hpp"
#include "../00-headers/03-cgi/cgi_handler.hpp"
#include <sys/select.h>

int findServerIndex(std::string const& request, std::vector<Server *>& Servers) 
{
    std::string host;
    int port = 8080; 

    size_t hostPos = request.find("Host: ");
    if (hostPos != std::string::npos) 
    {
        hostPos += 6;  
        size_t endPos = request.find("\r\n", hostPos);
        std::string hostPort = request.substr(hostPos, endPos - hostPos);        
        size_t colonPos = hostPort.find(":");
        if (colonPos != std::string::npos) 
        {
            host = hostPort.substr(0, colonPos);
            std::istringstream iss(hostPort.substr(colonPos + 1));
            iss >> port;
        } 
        else 
            host = hostPort;
    }
    for (size_t i = 0; i < Servers.size(); ++i) 
    {
        const std::vector<int>& serverPorts = Servers[i]->getPorts(); 
        if (Servers[i]->getConf()->getConfig("host") == host && 
            std::find(serverPorts.begin(), serverPorts.end(), port) != serverPorts.end()) 
            return i;
    }
    return 0;
}

void type_request_manager(int *fd_client, std::string *req, Server *server, SessionManager &sessionManager)
{            
    HttpRequest request(*req, server);
    HttpResponse response(request);
    std::ostringstream msg_size;
    msg_size << request.getBody().size();
    msg_size << " bytes";
    

    if (request.getMethod().size() == 0)
    {
        response.setRedirection(400);
        response.send(*fd_client);
        return;
    }
    if (req->find("HTTP/1.1") == std::string::npos)
    {
        response.setRedirection(505);
        response.send(*fd_client);
        return;
    }
    if (!request.isValidBodySize())
    {
        response.setRedirection(413);
        response.send(*fd_client);
        return;
    }
    Log::print_final_log("Request:", request.getMethod(), request.getURI(), msg_size.str());
    if (request.getMethod() == "PUT" && !request.isScript())
    {
        response.put(request);
        response.setStatusCode(201);
        response.send(*fd_client);
    }
    response.setResourcePath(request);
 
   /* if (!request.isStatic())
    {*/
        std::string cookieHeader = request.getHeader("Cookie");
        Cookies cookies(cookieHeader);
        std::string sessionId = cookies.getCookie("sessionId");
        if ((!sessionManager.sessionExist(sessionId) || sessionId.empty()))
        {
            sessionId = sessionManager.createSessions();
            cookies.setCookie("sessionId", sessionId);
        }
        response.addHeader("Set-Cookie", cookies.getSetCookieHeader().substr(12));
   /* }*/
    std::string filePath = response.getFilePath();
    filePath = removeDuplicateSlashes(filePath);            
    if ((filePath.find("cgi") != std::string::npos || filePath.find(".cgi") != std::string::npos || request.hasFileSpecialRoute(getExtension(filePath)))) 
    {   
        Location *route = NULL;
        if (request.hasFileSpecialRoute(getExtension(filePath)))
            route = server->getRoute(getExtension(filePath));
        else if (filePath.find("cgi") != std::string::npos || filePath.find(".cgi") != std::string::npos)
            route = server->getRoute("/cgi-bin/");
        if (!route)
            return;
        else if (response.isAllowedMethod(route, request))
        {
            filePath = route->root() + request.getAskedFile();
            Cgi_handler cgiHandler(*fd_client, &request, &response, route);
            cgiHandler.executeCGI(filePath);
        }
        else
        { 
            response.setRedirection(406);
            response.send(*fd_client);
        }
    }
    else if (filePath.find("cgi-bin/") != std::string::npos && !server->getCgiStatus())
    { 
        response.setRedirection(403);
        response.send(*fd_client);
    }
    else   
        response.send(*fd_client);
    std::string connectionHeader = request.getHeader("Connection");
    req->clear();
}

void request_and_response_fd_manager(std::vector<int>& fd_sockets, std::vector<Server*>& Servers, SessionManager &sessionManager)
{
    RequestResponseManager rrm(fd_sockets, Servers, sessionManager);
    rrm.run();
}
