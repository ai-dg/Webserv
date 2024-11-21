/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stringUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:59 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 20:30:02 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/02-utils/stringUtils.hpp"

std::string replaceBy(std::string original, std::string find, std::string replace)
{
    int pos = original.find(find);
    original.erase(pos, find.size());
    original.insert(pos, replace);
    return original;
}

std::pair<std::string, std::string> split(std::string const& str, char delimiter) 
{
    size_t pos = str.find(delimiter);
    return std::pair<std::string, std::string>(str.substr(0, pos), str.substr(pos + 1));
}

std::string itos(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}

int stoi(std::string str)
{
    int number;
    std::stringstream ss;
    ss << str;
    ss >> number;
    return number;
}

std::string trim(std::string str)
{
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    size_t last =  str.find_last_not_of(" \t\n\r\f\v");
    
    if (first == std::string::npos)
        return "";
    return (str.substr(first, last - first + 1));
}

std::string upperCaseMe(std::string str)
{
    int i = 0;
    std::string cpy(str);

	while (cpy[i] != '\0')
	{
		cpy[i] = toupper(cpy[i]);
        i++;
	}
    return cpy;
}

std::string extractLastSegment(const std::string& path) 
{
    size_t lastSlashPos = path.find_last_of('/');
    if (lastSlashPos == std::string::npos || lastSlashPos == 0) 
        return path;

    size_t secondLastSlashPos = path.find_last_of('/', lastSlashPos - 1);
    if (secondLastSlashPos != std::string::npos) 
        return path.substr(secondLastSlashPos, lastSlashPos - secondLastSlashPos + 1);
    else 
        return path.substr(0, lastSlashPos + 1);
}

std::string removeDuplicateSlashes(const std::string& path) 
{
    std::string result;
    bool lastWasSlash = false;

    for (size_t i = 0; i < path.size(); ++i) 
    {
        if (path[i] == '/') 
        {
            if (!lastWasSlash) 
            {
                result += path[i];
                lastWasSlash = true;
            }
        }
        else 
        {
            result += path[i];
            lastWasSlash = false;
        }
    }
    return result;
}
