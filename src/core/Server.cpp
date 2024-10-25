/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:47:22 by ls                #+#    #+#             */
/*   Updated: 2024/10/25 12:32:33 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"

Server::Server()
{
    std::cout << "server on" << std::endl;    
    host_ip.s_addr = htonl(INADDR_LOOPBACK);
}

Server::Server(Conf &c)
{
    this->conf = c;
}

std::string Server::getHostipv4()
{
    return std::string(inet_ntoa(host_ip));
}

Server::Server(std::string path)
{
    (void) path;
    std::string line;
    std::cout << "param server on" << std::endl;
    std::ifstream config(path.c_str());
    if (!config.is_open())
        return; /////////////////// wrong way ----- have to getout properly...
    while (std::getline(config, line))
    {
        parseConfig(line);
        //std::cout << line << std::endl;
    }
    config.close();
    
}



void Server::setKeepAlive(std::string line)
{
    line = trim(line, ' ');
    int spacePos = line.find(" ");
    std::string str_time;
    if (spacePos != std::string::npos)
    {
        //a proteger...
        str_time = line.substr(spacePos + 1, std::string::npos);
        char* end;
        this->keepAlive = std::strtol(str_time.c_str(), &end, 10);
    }      
    else
        this->keepAlive = 60;
}

void Server::parseConfig(std::string line)
{
    if (line.find("keepalive_timeout") != std::string::npos)
        this->setKeepAlive(line);
}

void Server::setPort(int port)
{
    this->port = port;
    std::cout << "listening on port " << this->port << std::endl;    
}

int Server::getPort(void)
{
    return (this->port);
}

