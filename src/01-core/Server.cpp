/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:47:22 by ls                #+#    #+#             */
/*   Updated: 2024/11/21 20:14:29 by dagudelo         ###   ########.fr       */
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
    int multi = 1;   
    std::stringstream stream;
    std::string mbs = trim(conf->getConfig("client_max_body_size"));
    stream << mbs;
    try {
        if (mbs[mbs.size() - 1] == 'M')
            multi = 1024;
        else if (mbs[mbs.size() - 1] == 'K')
            multi = 1;
        else
            throw InvalidArgException();
        int max ;
        stream >> max;
        maxBodySize = max * multi;
        char unit = 'K';
        if (multi > 1)
            unit = 'M';
        Log::output("./sessions/Server.txt") << "MAX BODY SIZE SET TO : " << maxBodySize << unit << std::endl;
    }
    catch (const InvalidArgException &e)
    {
        maxBodySize = 2048;
        Log::output("./sessions/Server.txt") << e.what() << ": client_max_body_size value set to " << maxBodySize << std::endl;
    }
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
                Log::output("./sessions/Server.txt") <<host_names << std::endl;
                spacepos = host_names.find_first_of(" \t");
                if (spacepos == std::string::npos)
                {   
                    Hosts.push_back(host_names);
                    break;

                }
            }
        }
    }
    Log::output("./sessions/Server.txt") << "BUG" << std::endl;
    Log::output("./sessions/Server.txt") << BOLD_RED << "HOST NAMES ::::::::::::::::::::::::::::: " << RESET << std::endl;
    printContenerValues(Hosts, BOLD_RED);    
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
        {
            ports.push_back(portNumber);
        } 
        else 
            Log::output("./logs/error.log") << "Port invalide dans la configuration : " << portNumber << std::endl;  
    }
    if (inet_pton(AF_INET, (conf->getConfig("host")).c_str(), &host_ip) < 0)
    {
        perror("invalid host");
        Log::error("Invalid host : check your configuration file");
    }
    else
    {
        Log::output("./sessions/Server.txt") << BOLD_GREEN << "Server on" << RESET << std::endl;
        Log::output("./sessions/Server.txt") << "listening " << conf->getConfig("host") << " on ports ";
        std::vector<int>::iterator it;
        for (it = ports.begin(); it != ports.end(); it++)
        {
            Log::output("./sessions/Server.txt") << *it << " ";
        }
        Log::output("./sessions/Server.txt") << std::endl; 
    } 
    setMaxBodySize();
    setHostNames();
    Log::output("./sessions/Server.txt") << "Server class object created" << std::endl;
    conf->printRoutesConfig(id);
    serverNumber++;
}

Server::Server(Server const& src) : id(src.id), keepAlive(src.keepAlive), conf(src.conf), maxBodySize(src.maxBodySize), host_ip(src.host_ip), methods(src.methods), err(src.err), Hosts(src.Hosts), ports(src.ports)
{
    Log::output("./sessions/Server.txt") << "Server classs object copied" << std::endl;
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
    Log::output("./sessions/Server.txt") << "Server class object assigned" << std::endl;
    return *this;
}

Server::~Server()
{
    Log::output("./sessions/Server.txt") << "Server class object destroyed" << std::endl;
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
