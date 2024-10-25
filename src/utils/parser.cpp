/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 15:32:55 by calbor-p          #+#    #+#             */
/*   Updated: 2024/10/25 13:30:34 by calbor-p         ###   ########.fr       */
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
