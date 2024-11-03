/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:47:22 by ls                #+#    #+#             */
/*   Updated: 2024/10/26 08:28:00 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"
#include "../headers/Conf.hpp"
#include "../headers/parser.hpp"
#include "../headers/colors.hpp"
#include "../headers/defines.hpp"
#include "../headers/InvalidArgException.hpp"
#include "../headers/debugTools.hpp"
#include <iostream>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <sstream>


/**
 * @brief Public:
 */
Server::Server()
{
    Log::output("./sessions/Server.txt") << "server on" << std::endl;    
    host_ip.s_addr = htonl(INADDR_LOOPBACK);
}

size_t Server::getMaxBodySize()
{
    return maxBodySize;
}

void Server::setMaxBodySize()
{
    int multi = 1;   
    std::stringstream stream;
    std::string mbs = trim(conf.getConfig("client_max_body_size"));
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

Server::Server(Conf const& c) : conf(c)
{
    std::vector<std::string> listenPorts = conf.getListenPorts();

    //Log::output("./sessions/Server.txt") << "Nbr de ports : " << listenPorts.size() << std::endl;

    for (size_t i = 0; i < listenPorts.size(); ++i) 
    {
        int portNumber = atoi(listenPorts[i].c_str());
        if (portNumber >= 1 && portNumber <= 65535) 
        {
            //port[numPorts++] = portNumber;
            ports.push_back(portNumber);
        } 
        else 
        {
            Log::output("./logs/error.log") << "Port invalide dans la configuration : " << portNumber << std::endl;
        }        
    }
    
    if (inet_pton(AF_INET, (conf.getConfig("host")).c_str(), &host_ip) < 0)
    {
        perror("invalid host");
        Log::error("Invalid host : check your configuration file");
    }
    else
    {
        Log::output("./sessions/Server.txt") << BOLD_GREEN << "Server on" << RESET << std::endl;
        Log::output("./sessions/Server.txt") << "listening " << conf.getConfig("host") << " on ports ";
        std::vector<int>::iterator it;
        for (it = ports.begin(); it != ports.end(); it++)
        {
            Log::output("./sessions/Server.txt") << *it << " ";
        }
        /*for (int i = 0;  i < numPorts; i++)
        {
            Log::output("./sessions/Server.txt") << port[i] << " ";
        }*/

        Log::output("./sessions/Server.txt") << std::endl; 
    } 
    setMaxBodySize();
    setHostNames();
}

void Server::setHostNames()
{
    std::string host_names = conf.getConfig(HOST_NAMES);
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

Server::~Server()
{
    Log::output("./sessions/Server.txt") << "Server destroyed" << std::endl;
}

bool Server::getCgiStatus()
{
    if (conf.getConfig("location_/cgi-bin/cgi") == "on" )
        return true;
    return false;
}

void Server::addPort(int port)
{
    
    ports.push_back(port);
    //this->port[numPorts + 1] = port;
    //numPorts++;
    //Log::output("./sessions/Server.txt") << "listening on port " << this->port << std::endl;    
}

in_addr_t Server::getAddr()
{
    return (host_ip.s_addr);
}

std::string Server::getHostipv4()
{
    return std::string(inet_ntoa(host_ip));
}

std::vector<int>Server::getPorts()
{
    return ports;
}

Conf Server::getConf() const
{
    return this->conf;
}

/*
int* Server::getPorts(int& count)
{
    count = this->numPorts;
    return this->port;
}*/
/*
int Server::getNumPorts() const
{
    return this->numPorts;
}*/

