#include "../headers/Cookies.hpp"
#include <string>
#include <iostream>
#include <sstream>

/**
 * @brief Public:
 */
Cookies::Cookies(std::string const& cookieHeader) 
{
    parseCookies(cookieHeader);
    std::cout << "Cookies created" << std::endl;
}

Cookies::~Cookies() 
{
    std::cout << "Cookies destroyed" << std::endl;
}

std::string Cookies::getCookie(std::string const& name) 
{
    std::map<std::string, std::string>::iterator it = cookies.find(name);
    if (it != cookies.end()) 
    {
        return it->second;
    }
    return "";
}

void Cookies::setCookie(std::string const& name, std::string const& value) 
{
    cookies[name] = value;
}

std::string Cookies::getSetCookieHeader() 
{
    std::string header;
    for (std::map<std::string, std::string>::const_iterator it = cookies.begin(); it != cookies.end(); ++it) 
    {
        header += "Set-Cookie: " + it->first + "=" + it->second + "; Path=/; HttpOnly\r\n";
    }
    return header;
}

/**
 * @brief Private:
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

            cookies[name] = value;
        }
    }
}
