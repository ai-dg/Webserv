/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 15:34:03 by calbor-p          #+#    #+#             */
/*   Updated: 2024/10/24 14:47:59 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "Server.hpp"
#include "Conf.hpp"
#include <string>

void setServer(Conf const& conf, Server *server);
std::string trim(std::string str, char c);
std::string searchValueInFile(std::string path, std::string index);
void setPort(std::string listen, Server *server);
bool isValidConfFile(std::string path);

#endif