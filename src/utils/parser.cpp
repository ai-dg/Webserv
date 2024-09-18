/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 15:32:55 by calbor-p          #+#    #+#             */
/*   Updated: 2024/09/18 16:44:48 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <fstream>

#include "../headers/parser.hpp"

std::string trim(std::string str, char c)
{
	int start = 0;

	while( str[start] == c)
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
	std::cout << listen << std::endl;
	listen.
	std::string num = listen.substr(listen.find(" "), std::string::npos);
	std::cout << num << std::endl;
	
	server->setPort(port);
	
}

void setServer(std::string path, Server *server)
{
	std::ifstream config(path.c_str());	
	
	setPort(searchValueInFile(path, "listen"), server);
	std::cout << "parsing server config..." << std::endl;
	
	std::cout << "done..." << std::endl;
}