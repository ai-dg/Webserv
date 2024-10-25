/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:47:59 by ls                #+#    #+#             */
/*   Updated: 2024/10/25 13:39:14 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "includes.hpp"
#include "Conf.hpp"
#include <map>
#include <netinet/in.h>
#include "Conf.hpp"
#include "Log.hpp"
#include <string>

class Server
{
    private:
        int port[65535];
        int numPorts;
        struct in_addr host_ip;
        Conf conf;
        std::string serverName;
        std::string *methods;
        int keepAlive;
        std::map<int, std::string> err;
        int maxBodySize;
    
    public:
        Server();
        Server(Conf const& c);
        ~Server();
        void addPort(int port);
        in_addr_t getAddr();
        std::string getHostipv4();
        int* getPort(int& count);
        int getNumPorts() const;
};

#endif