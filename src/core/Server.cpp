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
}

Server::~Server()
{
    std::cout << "Server destroyed" << std::endl;
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
