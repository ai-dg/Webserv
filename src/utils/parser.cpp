/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 15:32:55 by calbor-p          #+#    #+#             */
/*   Updated: 2024/09/18 17:38:42 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <fstream>
#include <cstdlib>

#include "../headers/parser.hpp"

std::string trim(std::string str, char c)
{
	int start = 0;

	if (str == "")
		return str;
	while(str[start] == c)
		start++;
	return str.substr(start, std::string::npos);
}

std::string searchValueInFile(std::string path, std::string index)
{
	std::ifstream config(path.c_str());
	std::string line;
	while(std::getline(config, line))
	{
		if (line.find(index) != std::string::npos)
		{
			config.close();
			return line;
		}
	};
	config.close();
	return "";
}

void setPort(std::string listen, Server *server)
{
	int port = 8080;
	listen = trim(listen, ' ');
	int num = atoi((listen.substr(listen.find(" ") + 1, std::string::npos)).c_str());
	if (num >= 1024 && num <= 49151)
		port = num;		
	server->setPort(port);	
}

void setServer(std::string path, Server *server)
{
	std::ifstream config(path.c_str());	
	
	setPort(searchValueInFile(path, "listen"), server);
	std::cout << "parsing server config..." << std::endl;
	
	std::cout << "done..." << std::endl;
}