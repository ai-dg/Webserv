/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:47:22 by ls                #+#    #+#             */
/*   Updated: 2024/09/18 17:46:14 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"

Server::Server()
{
    std::cout << "server on" << std::endl;
    hostipv4[0] = 0;
    hostipv4[1] = 0;
    hostipv4[2] = 0;
    hostipv4[3] = 0;
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

Server::Server(std::string path)
{
    (void) path;
     std::cout << "param server on" << std::endl;
    
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