/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stringUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:59 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/05 05:32:48 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/02-utils/stringUtils.hpp"
#include "../00-headers/02-utils/Log.hpp"
#include "../00-headers/04-exceptions/InvalidArgException.hpp"

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


std::string getExtension(std::string filePath)
{
    if (filePath == "/" || filePath.find(".") == std::string::npos)
        return "";
    return filePath.substr(filePath.find("."), std::string::npos);
}

size_t getNextof(std::string str, size_t pos, char c)
{
    size_t i = pos;
    for (; i < str.size(); ++i)
    {
        if (str[i] == c)
            return i;        
    }
    return std::string::npos;    
}

size_t getFormatedSizeFromString(std::string size) {
    size_t multi = 1;
    size_t maxBodySize = 0;
    std::string mbs = trim(size);
    size_t decimalPointCount = 0;

    
    for (size_t i = 0; i < mbs.size(); ++i) 
        if (mbs[i] == '.') 
            ++decimalPointCount;

    if (decimalPointCount > 1) 
    {
        std::cerr << "Error: Invalid format - multiple decimal points in size string: " << size << std::endl;
        return 2048; 
    }
    try 
    {
        if (mbs[mbs.size() - 1] == 'M') 
        {
            multi = 1024 * 1024; 
            mbs = mbs.substr(0, mbs.size() - 1); 
        } 
        else if (mbs[mbs.size() - 1] == 'K') 
        {
            multi = 1024; 
            mbs = mbs.substr(0, mbs.size() - 1); 
        } 
        else 
            throw InvalidArgException();

        double max = 0.0; 
        max = atof(mbs.c_str()); 
        if (max <= 0.0) 
            throw InvalidArgException();

        maxBodySize = static_cast<size_t>(max * multi);
    } 
    catch (const InvalidArgException &e) 
    {
        std::cerr << "Error: Invalid size string format: " << size << std::endl;
        maxBodySize = 2048; 
    }
    return maxBodySize;
}

std::string cleanString(const std::string& str) 
{
    std::string cleaned;
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) 
        if (!std::isspace(static_cast<unsigned char>(*it))) 
            cleaned += *it;
    return cleaned;
}
