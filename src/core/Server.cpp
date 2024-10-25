/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:47:22 by ls                #+#    #+#             */
/*   Updated: 2024/10/25 17:26:51 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"
#include "../headers/Conf.hpp"
#include "../headers/parser.hpp"
#include <iostream>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * @brief Public:
 */
Server::Server()
{
    std::cout << "server on" << std::endl;    
    host_ip.s_addr = htonl(INADDR_LOOPBACK);
}

Server::Server(Conf &c)
{
    this->conf = c;
    port = atoi(conf.getConfig("listen").c_str());    
    if (inet_pton(AF_INET, (conf.getConfig("host")).c_str(), &host_ip) < 0)
    {
        perror("invalid host");
        Log::error("Invalid host : check your configuration file");
    }
    else
    {
        std::cout << "server on" << std::endl;    
        std::cout << "listening " << conf.getConfig("host") << " on port " << port << std::endl;
    } 
}

Server::~Server()
{
    std::cout << "Server destroyed" << std::endl;
}

void Server::setPort(int port)
{
    this->port = port;
    std::cout << "listening on port " << this->port << std::endl;    
}

in_addr_t Server::getAddr()
{
    std::cout << inet_ntoa(host_ip) <<std::endl;
    return (host_ip.s_addr);
}

std::string Server::getHostipv4()
{
    return std::string(inet_ntoa(host_ip));
}

int Server::getPort(void)
{
    return (this->port);
}

