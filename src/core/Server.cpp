/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:47:22 by ls                #+#    #+#             */
/*   Updated: 2024/09/18 16:30:47 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"

Server::Server()
{
    std::cout << "server on" << std::endl;
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