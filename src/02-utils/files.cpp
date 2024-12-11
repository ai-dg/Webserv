/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:23:00 by ls                #+#    #+#             */
/*   Updated: 2024/12/11 18:00:19 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/02-utils/files.hpp"
#include "../00-headers/02-utils/Log.hpp"

std::string getMime(const std::string& mime)
{
    std::ifstream file("config/mime.types");
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
                file.close();
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
            decoded += ' ';
        else 
            decoded += encoded[i];
    }
    return decoded;
}

std::string removePrefix(const std::string& input) 
{
    const std::string prefix = "./";
    if (input.size() >= prefix.size() && input.substr(0, prefix.size()) == prefix) 
        return input.substr(prefix.size());
    return input; 
}


/// @brief //////////// TRY with file to find.... to replace index.html
/// @param path 
/// @return /////
std::string getFile(const std::string& path)
{
    std::string path2 = removePrefix(path);
    std::string decodedPath = urlDecode(path2);
    std::string local;
    std::string content;

    if (!path2.empty() && path2[0] == '/') 
        local = "." + decodedPath;  
    else 
        local = "./" + decodedPath; 
    
    if (decodedPath.size() < 10)
        local += "index.html";
    std::ifstream file(local.c_str());
    std::string line;
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (!content.empty())
                content += "\n";
            content += line;
        }
        file.close();
        return content;
    }
    file.close();
    return FILENOTFOUND;
}

bool checkFormatOfConfig(std::string const& path_file)
{
    std::ifstream file(path_file.c_str());
    if (!file.is_open())
    {
        std::cerr << "Unable to open the file." << std::endl;
        return false;
    }
    std::string line;
    std::stack<std::string> blocks; 
    size_t lineNumber = 0;
    const char* validDirectives[] = {
        "listen", "host", "server_name", "root", "index", "methods",
        "error_page_403", "error_page_404", "error_page_405", "error_page_406", "error_page_413","error_page_500",
        "client_max_body_size", "max_body", "keepalive_timeout", "client_body_timeout",
        "client_header_timeout", "exe", "autoindex", "cgi", "cgi_bin", "extension",
        "return"
    };
    size_t directiveCount = sizeof(validDirectives) / sizeof(validDirectives[0]);

    while (std::getline(file, line))
    {
        lineNumber++;  
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty() || line[0] == '#')
            continue;

        if (line == "server {" || line.find("location ") == 0)
        {
            blocks.push("{");
            continue;
        }

        if (line == "}")
        {
            if (blocks.empty())
            {
                std::cerr << "Unmatched closing brace at line " << lineNumber << std::endl;
                return false;
            }
            blocks.pop();
            continue;
        }

        size_t semicolon = line.find(';');
        if (semicolon != std::string::npos)
        {
            std::string key = line.substr(0, line.find(' '));
            bool isValid = false;
            for (size_t i = 0; i < directiveCount; ++i)
            {
                if (key == validDirectives[i])
                {
                    isValid = true;
                    break;
                }
            }
            if (!isValid)
            {
                std::cerr << "Invalid directive at line " << lineNumber << ": " << key << std::endl;
                return false;
            }
            continue;
        }
        std::cerr << "Invalid syntax at line " << lineNumber << ": " << line << std::endl;
        return false;
    }

    if (!blocks.empty())
    {
        std::cerr << "Unmatched opening brace(s) in the configuration file." << std::endl;
        return false;
    }
    if (checkFormatOfPaths(path_file) == false)
        return false;
    file.close();
    return true;
}

bool checkFormatOfPaths(std::string const& path_file)
{
    std::ifstream file(path_file.c_str());
    if (!file.is_open())
    {
        std::cerr << "Unable to open the file for path verification." << std::endl;
        return false;
    }
    std::string line;
    size_t lineNumber = 0;
    const char* pathDirectives[] = { "root", "cgi_bin" };
    const char* ignoredDirectives[] = { "error_page_403", "error_page_404", "error_page_405", "error_page_413", "error_page_500" };
    size_t pathDirectiveCount = sizeof(pathDirectives) / sizeof(pathDirectives[0]);
    size_t ignoredDirectiveCount = sizeof(ignoredDirectives) / sizeof(ignoredDirectives[0]);

    while (std::getline(file, line))
    {
        lineNumber++;
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        if (line.empty() || line[0] == '#')
            continue;
        bool isIgnored = false;
        for (size_t i = 0; i < ignoredDirectiveCount; ++i)
        {
            std::string directive = ignoredDirectives[i];
            if (line.find(directive) == 0)
            {
                isIgnored = true;
                break;
            }
        }
        if (isIgnored)
            continue;
        for (size_t i = 0; i < pathDirectiveCount; ++i)
        {
            std::string directive = pathDirectives[i];
            if (line.find(directive) == 0)
            {  
                size_t valueStart = line.find(' ') + 1;
                size_t valueEnd = line.find(';', valueStart);
                if (valueStart == std::string::npos || valueEnd == std::string::npos)
                {
                    std::cerr << "Invalid syntax for directive '" << directive << "' at line " << lineNumber << std::endl;
                    return false;
                }
                std::string path = line.substr(valueStart, valueEnd - valueStart);
                if (!path.empty() && (path[0] != '/' || path[path.size() - 1] != '/'))
                {
                    std::cerr << "Path in directive '" << directive 
                              << "' at line " << lineNumber 
                              << " must start and end with '/'. Current path: " << path << std::endl;
                    return false;
                }
            }
        }
    }

    file.close();
    return true;
}

const char* PathNotCorrectFormat::what() const throw()
{
    return "Path file is not in the correct format.";
}

bool hasExtension(std::string file)
{
    return (file.find(".") != std::string::npos);    
}

bool isValidUri(std::string uri)
{
    if (uri.find("%20") != std::string::npos)
        return false;
    if (uri.find_first_of("<>\"#{}|\\^~[]`") !=std::string::npos)
        return false;
    for (size_t i = 0; i < uri.size(); i++)
    {
        if (uri[i] <= 0x1F || uri[i] == 0x7F)
            return false;
    }
    return true;
    
}

char *resolvePath(std::string path)
{
    char* resolvedPath;
    resolvedPath = realpath(path.c_str(), NULL);
    return resolvedPath;
}
