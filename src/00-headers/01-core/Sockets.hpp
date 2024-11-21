/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:56:15 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 18:56:16 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../00-shared/includes.hpp"
#include "../01-core/Conf.hpp"
#include "../01-core/Server.hpp"

void get_all_server_conf(std::string const& path, std::vector<Conf*>& Configs);
int start_all_servers(std::vector<int>& fd_sockets, std::vector<Server*>& Servers, std::vector<Conf*>& Configs);
int socket_start(std::vector<int>& fd_sockets, std::vector<int>& listPorts);
int setup_connection_socket(std::vector<int>& fd_sockets, std::vector<int>& listPorts);

