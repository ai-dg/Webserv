/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:47:59 by ls                #+#    #+#             */
/*   Updated: 2024/10/24 14:52:48 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "includes.hpp"
#include <map>

class Server
{
    private:
        int port;
        int hostipv4[4];
        std::string serverName;
        std::string *methods;
        int keepAlive;
        std::map<int, std::string> err;
        int maxBodySize;
        void setKeepAlive(std::string line);
        void parseConfig(std::string line);
    
    public:
        Server();
        Server(std::string path);
        void setPort(int port);
        void getHostipv4();
        int getPort(void);
};

#include "parser.hpp"


#endif