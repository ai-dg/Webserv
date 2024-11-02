/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:23:00 by ls                #+#    #+#             */
/*   Updated: 2024/10/24 12:39:09 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/files.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

std::string getMime(const std::string& mime)
{
    std::ifstream file("config/mime.types"); /// may need to change the path
    std::string line;
    std::string mime_type;
   
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (line.find('#') != std::string::npos || line.empty())
                continue;
            if (line.find(mime) != std::string::npos)
            {
                mime_type = line.substr(0,line.find(' '));
                return mime_type;
            }
        }
        file.close();        
    }
    return "";
}

std::string checkMimeType(const std::string& path)
{
    std::string local = path;
    std::cout << "path : " << path << std::endl;
    local.erase(0,8);
    std::cout << "local : " << local << std::endl;
    if (local == "/")
        return "text/html";
    
    std::string mime = local.substr(local.find_last_of(".") + 1);
    return getMime(mime);
}


std::string urlDecode(const std::string& encoded)
{
    std::string decoded;
    char hex[3];
    hex[2] = '\0';

    for (size_t i = 0; i < encoded.length(); ++i) 
    {
        if (encoded[i] == '%' && i + 2 < encoded.length()) 
        {
            hex[0] = encoded[i + 1];
            hex[1] = encoded[i + 2];
            std::istringstream iss(hex);
            int value;
            iss >> std::hex >> value;
            decoded += static_cast<char>(value);
            i += 2;
        } 
        else if (encoded[i] == '+') 
        {
            decoded += ' ';
        } 
        else 
        {
            decoded += encoded[i];
        }
    }
    
    return decoded;
}

std::string getFile(const std::string& path)
{
    std::string decodedPath = urlDecode(path);
    std::string local;

    if (!path.empty() && path[0] == '/') 
    {
        local = "." + decodedPath;  
    } 
    else 
    {
        local = "./" + decodedPath; 
    }
    
    if (decodedPath.size() < 10)
        local += "index.html";
    std::cout << "local 2 " << local << " - path size : " << decodedPath.size() << std::endl;
    std::ifstream file(local.c_str());
    std::string content;
    std::string line;
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (!content.empty())
                content += "\n";
            content += line;
        }
       // std::cout << "test getfile : " << content << std::endl;
        file.close();
        return content;
    }
    else
        std::cout << "file not found ! " << std::endl;
    return FILENOTFOUND;
}
