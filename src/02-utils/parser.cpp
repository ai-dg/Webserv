/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 15:32:55 by calbor-p          #+#    #+#             */
/*   Updated: 2024/12/05 05:31:39 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/Server.hpp"
#include "../00-headers/02-utils/Log.hpp"
#include "../00-headers/02-utils/parser.hpp"

void setServer(Conf const& conf, Server *server)
{
	(void)conf;
	(void)server;
}

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

bool isValidConfFile(std::string path)
{
	std::ifstream file(path.c_str());
	(void) file;
	return true;
	// verifier que le fichier existe...
	// verifier qu'il n'est pas vide
	// verifier qu'il y a au moins un bloc server
	// verifier que les port sont dans les normes...
	// vérifier la cohérences des accolades...
	// verifier qu'il y a autant d'accolades que de server et location
	// vérifier qu'au moins une location est set par server ???? a voir avec le sujet
}

