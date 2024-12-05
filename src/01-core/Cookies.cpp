/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:57:52 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/05 05:13:30 by dagudelo         ###   ########.fr       */
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
    std::istringstream stream(cookieHeader);
    std::string token;

    while (std::getline(stream, token, ';')) 
    {
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
                continue;
            }
            cookies[name] = value;
        }
    }
}

/**
 * @brief Coplien Form
 */
Cookies::Cookies(std::string const& cookieHeader) 
{
    parseCookies(cookieHeader);
}

Cookies::Cookies(Cookies const& src) : cookies(src.cookies)
{
}

Cookies& Cookies::operator=(Cookies const& src) 
{
    if (this != &src) 
    {
        cookies.clear();
        cookies = src.cookies;
    }
    return *this;
}

Cookies::~Cookies() 
{
}

/**
 * @brief Getters
 */

std::string Cookies::getCookie(std::string const& name) 
{
    std::map<std::string, std::string>::iterator it = cookies.find(name);
    if (it != cookies.end()) 
    {
        return it->second;
    }
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
            header += singleSetCookie;
            uniqueCookies[it->first] = it->second;
        }
    }
    return header;
}

/**
 * @brief Setters
 */
void Cookies::setCookie(std::string const& name, std::string const& value) 
{
    cookies[name] = value;
}
