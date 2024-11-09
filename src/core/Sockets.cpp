#include <iostream>
#include <unistd.h>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include "../headers/Sockets.hpp"
#include "../headers/Conf.hpp"
#include "../headers/Server.hpp"
#include "../headers/Log.hpp"

void get_all_server_conf(std::string const& path, std::vector<Conf>& Configs) 
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
    std::ofstream outfile_map("./sessions/server_map.txt");
    if (!outfile_map.is_open())
    {
        Log::error("Unable to open output file: sessions/server_map.txt");
        return;
    }
    std::ofstream outfile("./sessions/server_map_conf.txt");
    if (!outfile.is_open()) 
    {
        Log::error("Unable to open output file: sessions/server_map_conf.txt");
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
        
        else if (in_server_block) {
            server_block += line + "\n";
        }
    }

    index = 0;
    for (std::map<int, std::string>::iterator it = map_conf.begin(); it != map_conf.end(); ++it)
    {
        outfile_map << "map #" << index << ": " << std::endl << it->second << std::endl;
        index++;
    }

    index = 0;   
    for (std::map<int, std::string>::iterator it = map_conf.begin(); it != map_conf.end(); ++it)
    {
        
        std::ofstream temp_file("./config/temp_server_block.conf");
        std::string temp_file_path = "./config/temp_server_block.conf";
        temp_file << it->second;
        temp_file.close();
        
        
        Conf conf(temp_file_path);
        Configs.push_back(conf);
        remove("./config/temp_server_block.conf");
        index++;
    }

    for (size_t i = 0; i < Configs.size(); ++i) 
    {
        outfile << "Configuration du serveur " << i << " :" << std::endl;
        Configs[i].printConfigs(outfile);
        outfile << std::endl;
    }
    file.close();
    outfile_map.close();
    outfile.close();
}

int start_all_servers(std::vector<int>& fd_sockets, std::vector<Server>& Servers, std::vector<Conf>& Configs)
{
    int numServers = 0;
    std::vector<int> listPorts;


    for (std::vector<Conf>::iterator it = Configs.begin(); it != Configs.end(); ++it)
    {
        numServers++;
    }    
    for (int i = 0; i < numServers; i++)
    {
        /**
         * Server start
         */
        Server server(Configs[i]);
        Servers.push_back(server);

        for (size_t j = 0; j < server.getPorts().size(); j++)
        {
            int port = server.getPorts()[j];
            Log::output("./sessions/Sockets.txt") << "Port: " << port << std::endl;
            if (std::find(listPorts.begin(), listPorts.end(), port) == listPorts.end())
            {
                listPorts.push_back(port);

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
        int fd_socket = socket(AF_INET, SOCK_STREAM, 0);
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
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;  
    // addr.sin_addr.s_addr = inet_addr("127.0.0.2");

    for (size_t i = 0; i < listPorts.size(); ++i) 
    {
        int fd_socket = fd_sockets[i];
        int opt = 1;
                
        if (setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0) 
        {
            Log::error("setsockopt failed");
            close(fd_socket);
            return 1;
        }        
        addr.sin_port = htons(listPorts[i]);        
        if (bind(fd_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) 
        {
            Log::error("binding failed");
            close(fd_socket);
            return 1;
        }       
        if (listen(fd_socket, 10) < 0) 
        {
            Log::output("./logs/error.log") << "Failed to listen on port " << listPorts[i] << std::endl;
            close(fd_socket);
            return 1;
        }
    }
    return 0;
}
