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
#include "../headers/InvalidArgException.hpp"
#include <iostream>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>


/**
 * @brief Public:
 */
Server::Server()
{
    std::cout << "server on" << std::endl;    
    host_ip.s_addr = htonl(INADDR_LOOPBACK);
}

size_t Server::getMaxBodySize()
{
    return maxBodySize;
}

void Server::setMaxBodySize()
{
    maxBodySize = 2048;
    int multi = 1;
    std::string mbs = conf.getConfig("client_max_body_size");
    try {
        if (mbs[mbs.size() - 1] == 'M')
            multi = 1024;
        else if (mbs[mbs.size() - 1] == 'K')
            multi = 1;
        else
            throw InvalidArgException();
        int max = atoi((conf.getConfig("client_max_body_size")).c_str());
        maxBodySize = max * multi;
        char unit = 'K';
        if (multi > 1)
            unit = 'M';
        std::cout << "MAX BODY SIZE SET TO : " << maxBodySize << unit << std::endl;
    }
    catch (const InvalidArgException &e)
    {
        std::cout << e.what() << " Default value set to 3072" << std::endl;
    }
    
}

Server::Server(Conf const& c) : conf(c)
{
    std::vector<std::string> listenPorts = conf.getListenPorts();

    //std::cout << "Nbr de ports : " << listenPorts.size() << std::endl;

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
            std::cerr << "Port invalide dans la configuration : " << portNumber << std::endl;
        }        
    }
    
    if (inet_pton(AF_INET, (conf.getConfig("host")).c_str(), &host_ip) < 0)
    {
        perror("invalid host");
        Log::error("Invalid host : check your configuration file");
    }
    else
    {
        std::cout << BOLD_GREEN << "Server on" << RESET << std::endl;
        std::cout << "listening " << conf.getConfig("host") << " on ports ";
        std::vector<int>::iterator it;
        for (it = ports.begin(); it != ports.end(); it++)
        {
            std::cout << *it << " ";
        }
        /*for (int i = 0;  i < numPorts; i++)
        {
            std::cout << port[i] << " ";
        }*/

        std::cout << std::endl; 
    } 
    setMaxBodySize();
}

Server::~Server()
{
    std::cout << "Server destroyed" << std::endl;
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
    //std::cout << "listening on port " << this->port << std::endl;    
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
