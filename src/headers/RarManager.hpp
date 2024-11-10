#ifndef RARMANAGER_HPP
#define RARMANAGER_HPP

#include <vector>
#include "Server.hpp"
#include "SessionManager.hpp"
#include "Epoll.hpp"

int findServerIndex(std::string const& request, std::vector<Server *>& Servers);
void type_request_manager(int *fd_client, std::string *req, Server *server, Epoll *epoll, SessionManager &sessionManager);
void request_and_response_fd_manager(std::vector<int>& fd_sockets, std::vector<Server *>& Servers, SessionManager &sessionManager);

#endif