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
    return NULL;
}

std::string checkMimeType(const std::string& path)
{
    std::string local = path;
    std::cout << "path : " << path << std::endl;
    local.erase(0,9);
    std::cout << "local : " << local << std::endl;
    if (local == "/")
        return "text/html";
    
    std::string mime = local.substr(local.find_last_of(".") + 1);
    return getMime(mime);
}

std::string getFile(const std::string& path)
{
    std::string local = path;
    
    if (path.size() < 10)
        local += "index.html";
    std::cout << "local 2 " << local << " - path size : " << path.size() << std::endl;
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
