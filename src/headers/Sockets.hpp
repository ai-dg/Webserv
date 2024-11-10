#ifndef SOCKETS_HPP
#define SOCKETS_HPP

#include <string>
#include <vector>
#include "Conf.hpp"
#include "Server.hpp"

void get_all_server_conf(std::string const& path, std::vector<Conf*>& Configs);
int start_all_servers(std::vector<int>& fd_sockets, std::vector<Server*>& Servers, std::vector<Conf*>& Configs);
int socket_start(std::vector<int>& fd_sockets, std::vector<int>& listPorts);
int setup_connection_socket(std::vector<int>& fd_sockets, std::vector<int>& listPorts);

#endif