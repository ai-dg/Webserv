/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:55:18 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/12 23:44:55 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_HPP
#define DEFINES_HPP

#include "colors.hpp"
#define CRLF "\r\n"
#define FILENOTFOUND "FILE NOT FOUND"
#define PATH_NOT_FOUND "PATH NOT FOUND"
#define PATH_FOUND "PATH FOUND"
#define BUFFER_SIZE 2048
#define FD_RESERVE 10
#define AUTO 200
#define NO_BODY 1
#define BODY 2
#define HOST "host"
#define HOST_NAMES "server_name"
#define DEFAULT_SERVER "default_server"
#define ERROR_PAGE_404 "error_page_404"
#define ERROR_PAGE_500 "error_page_500"
#define CLIENT_BODY_SIZE "client_max_body_size"
#define KEEPALIVE "keepalive_timeout"
#define CLIENT_BODY_TIMEOUT "client_body_timeout"
#define CLIENT_HEADER_TIMEOUT "client_header_timeout"
#define LOCATION_ROOT "location_/root"
#define LOCATION_ROOT_INDEX "location_/index"
#define DEBUG_MSG std::cerr << RED <<
#define END_DEBUG << RESET << std::endl;

#endif