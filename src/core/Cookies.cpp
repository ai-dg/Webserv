#include "../headers/Cookies.hpp"
#include <string>
#include <iostream>
#include <sstream>

/**
 * @brief Public:
 */
Cookies::Cookies(std::string const& cookieHeader) 
{
    std::cout << "Creating Cookies instance with header: " << cookieHeader << std::endl;
    parseCookies(cookieHeader);
    std::cout << "Cookies parsed successfully" << std::endl;
}

Cookies::~Cookies() 
{
    std::cout << "Cookies destroyed" << std::endl;
}

std::string Cookies::getCookie(std::string const& name) 
{
    std::cout << "Retrieving cookie with name: " << name << std::endl;
    std::map<std::string, std::string>::iterator it = cookies.find(name);
    if (it != cookies.end()) 
    {
        std::cout << "Cookie found: " << name << " = " << it->second << std::endl;
        return it->second;
    }
    std::cout << "Cookie not found: " << name << std::endl;
    return "";
}

void Cookies::setCookie(std::string const& name, std::string const& value) 
{
    std::cout << "Setting cookie: " << name << " = " << value << std::endl;
    cookies[name] = value;
}

std::string Cookies::getSetCookieHeader() 
{
    std::string header;
    std::map<std::string, std::string> uniqueCookies;
    
    for (std::map<std::string, std::string>::const_iterator it = cookies.begin(); it != cookies.end(); ++it) 
    {
        if (uniqueCookies.find(it->first) == uniqueCookies.end()) 
        {
            std::string singleSetCookie = "Set-Cookie: " + it->first + "=" + it->second + "; Path=/; HttpOnly\r\n";
            std::cout << "Adding to Set-Cookie header: " << singleSetCookie << std::endl;
            header += singleSetCookie;
            uniqueCookies[it->first] = it->second;
        }
    }
    std::cout << "Generated Set-Cookie header: " << header << std::endl;
    return header;
}

/**
 * @brief Private:
 */
void Cookies::parseCookies(std::string const& cookieHeader) 
{
    std::cout << "Parsing cookies from header: " << cookieHeader << std::endl;
    std::istringstream stream(cookieHeader);
    std::string token;

    while (std::getline(stream, token, ';')) 
    {
        std::cout << "Raw token from cookie header: " << token << std::endl;
        
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
                std::cout << "Ignoring invalid cookie entry: " << name << std::endl;
                continue;
            }

            cookies[name] = value;
            std::cout << "Parsed cookie: " << name << " = " << value << std::endl;
        }
    }
    std::cout << "Finished parsing cookies." << std::endl;
}
