/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:47:22 by ls                #+#    #+#             */
/*   Updated: 2024/10/24 14:55:29 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"
#include "../headers/Conf.hpp"
#include "../headers/parser.hpp"
#include <iostream>
#include <cstdlib>

/**
 * @brief Public:
 */
Server::Server()
{
    std::cout << "Server on" << std::endl;
    hostipv4[0] = 0;
    hostipv4[1] = 0;
    hostipv4[2] = 0;
    hostipv4[3] = 0;
}

Server::Server(Conf const& conf)
{
    std::string values = conf.getConfig("host");
    size_t start = 0;
    size_t end;
    int index = 0;

    while ((end = values.find('.', start)) != std::string::npos && index < 4) {
        hostipv4[index++] = std::atoi(values.substr(start, end - start).c_str());
        start = end + 1;
    }
    // Dernier segment
    if (index < 4) {
        hostipv4[index] = std::atoi(values.substr(start).c_str());
    }


    // (void) path;

    // std::string line;
    // std::cout << "param server on" << std::endl;
    // std::ifstream config(path.c_str());
    // if (!config.is_open())
    //     return; /////////////////// wrong way ----- have to getout properly...
    // while (std::getline(config, line))
    // {
    //     parseConfig(line);
    //     //std::cout << line << std::endl;
    // }
    // config.close();
    
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

void Server::getHostipv4()
{
    int i = 0;
    while (i < 4)
    {
        if (i == 3)
            std::cout << this->hostipv4[i] << std::endl;
        else
            std::cout << this->hostipv4[i] << ".";
        i++;
    }

}

int Server::getPort(void)
{
    return (this->port);
}

/**
 * @brief Private:
 */
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

void Server::parseConfig(Conf const& conf)
{
    this->setKeepAlive(conf.getConfig("keepalive_timeout"));

    // if (line.find("keepalive_timeout") != std::string::npos)
    //     this->setKeepAlive(line);
}
