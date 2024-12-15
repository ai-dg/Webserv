/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:47:22 by ls                #+#    #+#             */
/*   Updated: 2024/12/13 01:11:40 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/Server.hpp"
#include "../00-headers/01-core/Conf.hpp"
#include "../00-headers/02-utils/parser.hpp"
#include "../00-headers/02-utils/debugTools.hpp"
#include "../00-headers/04-exceptions/InvalidArgException.hpp"

/**
 * @brief Setters
 */
void Server::setMaxBodySize()
{
    maxBodySize = getFormatedSizeFromString(conf->getConfig("client_max_body_size"));
}

void Server::setHostNames()
{
    std::string host_names = conf->getConfig(HOST_NAMES);
    host_names = trim(host_names);

    if (host_names.size() == 0)
    {
        Hosts.push_back(DEFAULT_SERVER);
    }
    else
    {
        size_t spacepos = host_names.find_first_of(" \t");
        if (spacepos == std::string::npos)
            Hosts.push_back(host_names);
        else
        {
            while (spacepos != std::string::npos)
            {
                Hosts.push_back(host_names.substr(0, spacepos));
                host_names.erase(0, spacepos+1);
                spacepos = host_names.find_first_of(" \t");
                if (spacepos == std::string::npos)
                {   
                    Hosts.push_back(host_names);
                    break;

                }
            }
        }
    }   
}

/**
 * @brief Copelien Form
 */
Server::Server(Conf *c)
{
    static int serverNumber = 1;
    conf = c;
    id = serverNumber;
    std::vector<std::string> listenPorts = conf->getListenPorts();
    for (size_t i = 0; i < listenPorts.size(); ++i) 
    {
        int portNumber = atoi(listenPorts[i].c_str());
        if (portNumber >= 1 && portNumber <= 65535) 
            ports.push_back(portNumber);
        else 
            Log::output("./logs/error.log") << "Port invalide dans la configuration : " << portNumber << std::endl;  
    }
    if (inet_pton(AF_INET, (conf->getConfig("host")).c_str(), &host_ip) < 0)
    {
        perror("invalid host");
        Log::error("Invalid host : check your configuration file");
    }
    setMaxBodySize();
    setHostNames();
    serverNumber++;
}

Server::Server(Server const& src) : id(src.id), keepAlive(src.keepAlive), conf(src.conf), maxBodySize(src.maxBodySize), host_ip(src.host_ip), methods(src.methods), err(src.err), Hosts(src.Hosts), ports(src.ports)
{

}

Server& Server::operator=(Server &server)
{
    if (this != &server)
    {
        this->id = server.id;
        this->keepAlive = server.keepAlive;
        this->conf = server.conf;
        this->maxBodySize = server.maxBodySize;
        this->host_ip = server.host_ip;
        this->methods = server.methods;
        this->err = server.err;
        this->Hosts = server.Hosts;
        this->ports = server.ports;
    }
    return *this;
}

Server::~Server()
{
    Log::cleanup();
}

/**
 * @brief Getters
 */
int Server::getId()
{
    return id;
}

bool Server::getCgiStatus()
{
    Location *Route = getRoute("/cgi-bin/");
    if (!Route)
        return false;
    if (Route->cgi() == "on" )
        return true;
    return false;
}

bool Server::getCgiStatus(std::string path)
{
    Location *Route = getRoute(getExtension(path));
    if (!Route)
        return false;
    if (Route->cgi() == "on" )
        return true;
    return false;
}

in_addr_t Server::getAddr()
{
    return (host_ip.s_addr);
}

Conf *Server::getConf() const
{
    if(!conf)
    {
        std::cerr << "no conf..." <<std::endl;
        return(NULL);
    }
    return conf;
}

size_t Server::getMaxBodySize()
{
    return maxBodySize;
}

std::string Server::getHostipv4()
{
    return std::string(inet_ntoa(host_ip));
}

std::vector<int>Server::getPorts()
{
    return ports;
}

Location *Server::getRoute(std::string const &routePath) const
{
    return conf->checkRoute(routePath);
}

/**
 * @brief Checkers
 */
bool Server::foundHostName(std::string hostname)
{
    std::vector<std::string>::iterator it;
    for (it = Hosts.begin(); it != Hosts.end(); ++it)
    {
        if (*it == hostname)
            return true;
    }
    return false;
}

/**
 * @brief Adders
 */
void Server::addPort(int port)
{
    ports.push_back(port);
}

void Server::addFdClient(int fd)
{
    fd_clients.push_back(fd);
}

void Server::removeFdClient(int fd)
{
    std::vector<int>::iterator it = std::find(fd_clients.begin(), fd_clients.end(), fd);
    if (it != fd_clients.end())
        fd_clients.erase(it);
}