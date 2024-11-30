/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:47:59 by ls                #+#    #+#             */
/*   Updated: 2024/11/29 21:20:52 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../00-shared/includes.hpp"
#include "../01-core/Conf.hpp"
#include "../02-utils/stringUtils.hpp"

class Server
{
    private:
        int id;
        int keepAlive;
        Conf *conf;
        size_t maxBodySize;
        struct in_addr host_ip;
        std::string *methods;
        std::map<int, std::string> err;
        std::vector<std::string> Hosts;
        std::vector<int> ports;

        /**
         * @brief Setters
         */
        void setMaxBodySize();
        void setHostNames();
    
    public:
        /**
         * @brief Copelien Form
         */
        Server(Conf *c);
        Server(Server const& src);
        Server& operator=(Server &server);
        ~Server();

        /**
         * @brief Getters
         */
        int getId();
        bool getCgiStatus();
        bool getCgiStatus(std::string path);
        in_addr_t getAddr();
        Conf *getConf() const;
        size_t getMaxBodySize();
        std::string getHostipv4();
        std::vector<int>getPorts();
        Location *getRoute(std::string const & route) const;

        /**
         * @brief Checkers
         */
        bool foundHostName(std::string hostname);
        
        /**
         * @brief Adders
         */
        void addPort(int port);
};
