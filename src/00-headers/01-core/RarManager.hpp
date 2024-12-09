/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RarManager.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:56:02 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 18:56:03 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../00-shared/includes.hpp"
#include "../01-core/Server.hpp"
#include "../01-core/SessionManager.hpp"
#include "../01-core/Epoll.hpp"

int findServerIndex(std::string const& request, std::vector<Server *>& Servers);
void type_request_manager(int *fd_client, std::string *req, Server *server, SessionManager &sessionManager);
void request_and_response_fd_manager(std::vector<int>& fd_sockets, std::vector<Server *>& Servers, SessionManager &sessionManager);
