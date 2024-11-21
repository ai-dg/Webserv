/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:57:52 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 18:57:53 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/Cookies.hpp"
#include "../00-headers/02-utils/Log.hpp"

/**
 * @brief Parse cookies from header
 */
void Cookies::parseCookies(std::string const& cookieHeader) 
{
    Log::output("./sessions/Cookies.txt") << "Parsing cookies from header: " << cookieHeader << std::endl;
    std::istringstream stream(cookieHeader);
    std::string token;

    while (std::getline(stream, token, ';')) 
    {
        Log::output("./sessions/Cookies.txt") << "Raw token from cookie header: " << token << std::endl;
        size_t pos = token.find('=');
        if (pos != std::string::npos) 
        {
            std::string name = token.substr(0, pos);
            std::string value = token.substr(pos + 1);
            name.erase(0, name.find_first_not_of(" "));
            name.erase(name.find_last_not_of(" ") + 1);
            value.erase(0, value.find_first_not_of(" "));
            value.erase(value.find_last_not_of(" ") + 1);
            if (name.find("Set-Cookie:") == 0 || name.find("Set-Cookie") != std::string::npos) 
            {
                Log::output("./sessions/Cookies.txt") << "Ignoring invalid cookie entry: " << name << std::endl;
                continue;
            }
            cookies[name] = value;
            Log::output("./sessions/Cookies.txt") << "Parsed cookie: " << name << " = " << value << std::endl;
        }
    }
    Log::output("./sessions/Cookies.txt") << "Finished parsing cookies." << std::endl;
}

/**
 * @brief Coplien Form
 */
Cookies::Cookies(std::string const& cookieHeader) 
{
    Log::output("./sessions/Cookies.txt") << "Creating Cookies instance with header: " << cookieHeader << std::endl;
    parseCookies(cookieHeader);
    Log::output("./sessions/Cookies.txt") << "Cookies parsed successfully" << std::endl;
}

Cookies::Cookies(Cookies const& src) : cookies(src.cookies)
{
    Log::output("./sessions/Cookies.txt") << "Copying Cookies instance" << std::endl;
}

Cookies& Cookies::operator=(Cookies const& src) 
{
    Log::output("./sessions/Cookies.txt") << "Assigning Cookies instance" << std::endl;
    if (this != &src) 
    {
        cookies.clear();
        cookies = src.cookies;
    }
    return *this;
}

Cookies::~Cookies() 
{
    Log::output("./sessions/Cookies.txt") << "Cookies destroyed" << std::endl;
}

/**
 * @brief Getters
 */

std::string Cookies::getCookie(std::string const& name) 
{
    Log::output("./sessions/Cookies.txt") << "Retrieving cookie with name: " << name << std::endl;
    std::map<std::string, std::string>::iterator it = cookies.find(name);
    if (it != cookies.end()) 
    {
        Log::output("./sessions/Cookies.txt") << "Cookie found: " << name << " = " << it->second << std::endl;
        return it->second;
    }
    Log::output("./sessions/Cookies.txt") << "Cookie not found: " << name << std::endl;
    return "";
}

std::string Cookies::getSetCookieHeader() 
{
    std::string header;
    std::map<std::string, std::string> uniqueCookies;
    
    for (std::map<std::string, std::string>::const_iterator it = cookies.begin(); it != cookies.end(); ++it) 
    {
        if (uniqueCookies.find(it->first) == uniqueCookies.end()) 
        {
            std::string singleSetCookie = "Set-Cookie: " + it->first + "=" + it->second + "; Path=/; HttpOnly";
            Log::output("./sessions/Cookies.txt") << "Adding to Set-Cookie header: " << singleSetCookie << std::endl;
            header += singleSetCookie;
            uniqueCookies[it->first] = it->second;
        }
    }
    Log::output("./sessions/Cookies.txt") << "Generated Set-Cookie header: " << header << std::endl;
    return header;
}

/**
 * @brief Setters
 */
void Cookies::setCookie(std::string const& name, std::string const& value) 
{
    Log::output("./sessions/Cookies.txt") << "Setting cookie: " << name << " = " << value << std::endl;
    cookies[name] = value;
}
