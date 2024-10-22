/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ls <ls@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:23:00 by ls                #+#    #+#             */
/*   Updated: 2024/09/09 19:45:47 by ls               ###   ########.fr       */
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
    if (path == "/")
        return "text/html";
    std::string mime = path.substr(path.find_last_of(".") + 1);
    return getMime(mime);
}

std::string getFile(const std::string& path)
{

    std::ifstream file(path.c_str());
    std::string content;
    std::string line;
    std::cout << "test path : " << path << std::endl;
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (!content.empty())
                content += "\n";
            content += line;
        }
        std::cout << "test getfile : " <<content << std::endl;
        file.close();
    }
    else
        std::cout << "file not found ! " << std::endl;
    return content;
}
