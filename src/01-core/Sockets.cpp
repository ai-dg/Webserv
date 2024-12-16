/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:28 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/16 10:35:21 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/Conf.hpp"
#include "../00-headers/01-core/Sockets.hpp"
#include "../00-headers/01-core/Server.hpp"
#include "../00-headers/02-utils/Log.hpp"

void get_all_server_conf(std::string const& path, std::vector<Conf*>& Configs) 
{
    std::map<int, std::string> map_conf;
    std::string line;
    std::string server_block;
    bool in_server_block = false;
    bool in_location_block = false;
    int server_index = 0;
    int index;

    std::ifstream file(path.c_str());
    if (!file.is_open()) 
    {
        Log::error("Unable to open file: " + path);
        return;
    }
    while (std::getline(file, line)) 
    {
        
        if (line.find("server {") != std::string::npos) 
        {
            in_server_block = true;
            server_block = line + "\n";
        } 
        else if (line.find("location") != std::string::npos)
        {
            in_location_block = true;
            server_block += line + "\n";
        }
        
        else if (in_server_block && line.find("}") != std::string::npos) 
        {
            if (in_location_block)
            {
                server_block += line + "\n";
                in_location_block = false;
                continue;
            }
            else
            {
                server_block += line + "\n";
                map_conf[server_index++] = server_block;  
                in_server_block = false;
                server_block.clear();
            }
        } 
        else if (in_server_block)
            server_block += line + "\n";
    }
    index = 0;   
    for (std::map<int, std::string>::iterator it = map_conf.begin(); it != map_conf.end(); ++it)
    {
        
        std::ofstream temp_file("./config/temp_server_block.conf");
        std::string temp_file_path = "./config/temp_server_block.conf";
        temp_file << it->second;
        temp_file.close();
        
        Conf *conf = new Conf(temp_file_path);
        Configs.push_back(conf);
        ::remove("./config/temp_server_block.conf");
        index++;
    }
    file.close();
}

int start_all_servers(std::vector<int>& fd_sockets, std::vector<Server *>& Servers, std::vector<Conf *>& Configs)
{
    std::vector<int> listPorts;

    std::vector<Conf*>::iterator it ;
    for (it = Configs.begin(); it != Configs.end(); ++it)
    {
        /**
         * @brief Server start
         */
        Server *server = new Server(*it);        
        Servers.push_back(server);
        for (size_t j = 0; j < server->getPorts().size(); j++)
        {
            int port = server->getPorts()[j];
            if (std::find(listPorts.begin(), listPorts.end(), port) == listPorts.end())
            {
                listPorts.push_back(port);
                Log::print_final_log("Server listening", server->getHostipv4(), port);
            }
        }
    }
    /**
     * @brief Reglages des connexion et communication "Sockets"
     */
    if (socket_start(fd_sockets, listPorts) > 0)
        return 1;

    if (setup_connection_socket(fd_sockets, listPorts) > 0)
        return 1;
    return 0;
}

int socket_start(std::vector<int>& fd_sockets, std::vector<int>& listPorts)
{
    /**
    *    int fd_socket = socket(AF_INET, SOCK_STREAM, 0);
    *    Creation d'un socket permettant la connextion
    *    l'option AF_INET permet de choisir le protocole de connexion Protocoles Internet IPv4
    *    l'option SOCK_STREAM permet de choisir le type de connexion TCP man : (
    *    SOCK_STREAM Support de dialogue garantissant l'intégrité, fournissant un flux de données binaires, 
    *    et intégrant un mécanisme pour les transmissions de données hors-bande. )
    */
    fd_sockets.clear();
    for (size_t i = 0; i < listPorts.size(); ++i)
    {
        int fd_socket = socket(PF_INET, SOCK_STREAM, 0);
        if (fd_socket == -1)
        {
            perror("socket");
            return (1);
        }
        fd_sockets.push_back(fd_socket);
    }
    return 0;    
}

int setup_connection_socket(std::vector<int>& fd_sockets, std::vector<int>& listPorts) 
{
    for (size_t i = 0; i < listPorts.size(); ++i) 
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        
        int fd_socket = fd_sockets[i];
        int opt = 1;
        if (setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0) 
        {
            Log::error("setsockopt failed");
            if (fd_socket != -1)
            {
                ::close(fd_socket);
                fd_socket = -1;
            }
            return 1;
        }        
        addr.sin_port = htons(listPorts[i]);        
        if (bind(fd_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) 
        {
            Log::error("binding failed");
            if (fd_socket != -1)
            {
                ::close(fd_socket);
                fd_socket = -1;
            }
            return 1;
        }       
        if (listen(fd_socket, 128) < 0) 
        {
            if (fd_socket != -1)
            {
                ::close(fd_socket);
                fd_socket = -1;
            }
            return 1;
        }
    }
    return 0;
}
