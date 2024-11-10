/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:47:59 by ls                #+#    #+#             */
/*   Updated: 2024/11/10 12:18:48 by calbor-p         ###   ########.fr       */
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
#include "stringUtils.hpp"

class Server
{
    private:
        //int port[65535];
        //int numPorts;
        struct in_addr host_ip;
        std::vector<std::string> Hosts;
        std::string *methods;
        int keepAlive;
        std::map<int, std::string> err;
        std::vector<int> ports;
        size_t maxBodySize;
        void setMaxBodySize();
        void setHostNames();
        Conf *conf;
    
    public:
        Server();
        Server(Conf *c);
        ~Server();
        void addPort(int port);
        bool foundHostName(std::string hostname);
        in_addr_t getAddr();
        size_t getMaxBodySize();
        Location *getRoute(std::string const & route) const;
        std::string getHostipv4();
        std::vector<int>getPorts();
        bool getCgiStatus();
        

        //int* getPorts(int& count);
        Conf *getConf() const;
        int getNumPorts() const;
};

#endif