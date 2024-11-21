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

#pragma once

#include "../00-shared/includes.hpp"
#include "../01-core/Server.hpp"
#include "../01-core/Conf.hpp"

void setServer(Conf const& conf, Server *server);
std::string trim(std::string str, char c);
std::string searchValueInFile(std::string path, std::string index);
void setPort(std::string listen, Server *server);
bool isValidConfFile(std::string path);

