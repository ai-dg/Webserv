/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Conf.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:57:49 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 18:57:50 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/Conf.hpp"
#include "../00-headers/02-utils/Log.hpp"
#include "../00-headers/02-utils/stringUtils.hpp"

/**
 * @brief Coplien Form
 */
Conf::Conf(std::string& path) : path(path)
{
    listenPorts.clear();
    configMap.insert(std::make_pair("host", ""));
    configMap.insert(std::make_pair("server_name", ""));
    configMap.insert(std::make_pair("error_page_404", ""));
    configMap.insert(std::make_pair("error_page_500", ""));
    configMap.insert(std::make_pair("client_max_body_size", ""));
    configMap.insert(std::make_pair("keepalive_timeout", ""));
    configMap.insert(std::make_pair("client_body_timeout", ""));
    configMap.insert(std::make_pair("client_header_timeout", ""));
    setLocations();
    getValuesFromPath();
    Log::output("./sessions/Conf.txt") << "path: " << this->path;
    std::ofstream file("./test.txt");
    printConfigs(file);
    checkAndSetDefaultValues();
    file.close();
    Log::output("./sessions/Conf.txt") << "Conf class object has been created" << std::endl;
}

Conf::Conf(Conf const& src) : configMap(src.configMap), routes(src.routes), path(src.path), listenPorts(src.listenPorts)
{
    Log::output("./sessions/Conf.txt") << "Conf class object has been copied" << std::endl;
}

Conf& Conf::operator=(Conf &conf)
{
    if (this != &conf)
    {
        configMap.clear();
        routes.clear();
        listenPorts.clear();
        this->configMap = conf.configMap;
        this->routes = conf.routes;
        this->path = conf.path;
        this->listenPorts = conf.listenPorts;
    }
    Log::output("./sessions/Conf.txt") << "Conf class object has been assigned" << std::endl;
    return *this;
}

Conf::~Conf()
{
    for (std::map<std::string, Location*>::iterator it = routes.begin(); it != routes.end(); ++it)
    {
        if (it->second) 
        {
            delete it->second; 
            it->second = NULL; 
        }
    }
    routes.clear(); 
    configMap.clear();         
    listenPorts.clear();       
    path.clear();              
    Log::output("./sessions/Conf.txt") << "Conf class object has been destroyed" << std::endl;
    Log::cleanup();
}

/**
 * @brief Print functions
 */
void Conf::printRoutesConfig(int servNb)
{
    std::cerr << RED << "server " << servNb << RESET << std::endl;
    std::map<std::string, Location*>::iterator it;
    for (it = routes.begin(); it != routes.end(); ++it)
        it->second->debugValues();
}

void Conf::printStatus(bool status, std::string text)
{
    if (status)    
        Log::output("./sessions/Conf.txt") << "[  "<< GREEN << "on" << RESET << "   ]  "<< text  << std::endl;
    else
        Log::output("./sessions/Conf.txt") << "[  "<< RED << "off" << RESET << "  ]  "<< text  << std::endl;
}

void Conf::printConfigs(std::ofstream& out) const
{ 
    std::map<std::string, std::string>::const_iterator it;
    out << "-------Config values from map---------" << std::endl;
    for (it = configMap.begin(); it != configMap.end(); ++it) 
    {
        out << it->first << ": ";
        if (it->second.empty())
            out << "NULL";
        else 
            out << it->second;
        out << std::endl;
    }
    out << "Ports to listen on ";
    out << listenPorts.size() << ": ";
    for(size_t i = 0; i < listenPorts.size(); ++i)
        out << listenPorts[i] << " ";
    out << std::endl;
    out << "--------------------------------------" << std::endl;
}

void Conf::printFile()
{
    std::ifstream confFile(path.c_str());
    std::string line;

    if (!confFile.is_open()) 
    {
        Log::output("./logs/error.log") << "Unable to open configuration file: " << path << std::endl;
        return;
    }
    while (std::getline(confFile, line)) 
        std::cerr <<BOLD_BLUE << line  << RESET << std::endl;
    confFile.close();
}

/**
 * @brief Getters
 */
void Conf::getValuesFromPath()
{
    std::ifstream confFile(path.c_str());
    std::string line;
    std::string currentLocation = ""; 
    std::string routPath;

    if (!confFile.is_open()) 
    {
        Log::output("./logs/error.log") << "Unable to open configuration file: " << path << std::endl;
        return;
    }
    while (std::getline(confFile, line)) 
    {
        line = trim(line);
        if (line.empty() || line[0] == '#') 
            continue;
        if (line.find("location") != std::string::npos) 
        {
            std::getline(confFile, line);
            while (line.find_first_of("}") == std::string::npos)
                std::getline(confFile, line);     
            continue;
        }
        if (line == "}") 
        {
            currentLocation = "";
            continue;
        }
        size_t pos = line.find(' ');
        if (pos != std::string::npos) 
        {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            if (!value.empty() && value[value.length() - 1] == ';')
                value.erase(value.length() - 1);
            if (key == "listen")
            {
                listenPorts.push_back(value);
                continue;
            }
            if (!currentLocation.empty()) 
            {   
                if (currentLocation[currentLocation.length() - 1] != '/') 
                    key = "location_" + currentLocation + "/" + key;
                else 
                    key = "location_" + currentLocation + key;
                
                key.erase(0, key.find_first_not_of(" \t")); 
                key.erase(key.find_last_not_of(" \t") + 1); 
            }
            Log::output("./sessions/Conf.txt") << "Key: " << key << ", Value: " << value << std::endl;
            if (hasKey(key)) 
                setConf(key, value);
            else 
                Log::output("./sessions/Conf.txt") << "Key not found: " << key << std::endl;
        }
    }
    confFile.close();
}

std::string Conf::getConfig(const std::string& key) const
{  
    std::map<std::string, std::string>::const_iterator it = configMap.find(key);
    if (it != configMap.end()) 
        return it->second;
    return ""; 
}

const std::vector<std::string>& Conf::getListenPorts() const
{
    return this->listenPorts;
}

/**
 * @brief Setters
 */
void Conf::setLocations()
{
    std::ifstream confFile(path.c_str());
    std::string line;
    std::string currentLocation = ""; 
    bool locationstatus = false;
    std::string routePath;

    if (!confFile.is_open()) 
    {
        Log::output("./logs/error.log") << "Unable to open configuration file: " << path << std::endl;
        return;
    }
    while (std::getline(confFile, line)) 
    {
        line = trim(line);
        if (line.empty() || line[0] == '#') 
            continue;  
        if (line.find("location") != std::string::npos) 
        {
            currentLocation.clear();
            size_t pos = line.find(" ") + 1;
            routePath = line.substr(pos, line.find_last_of(" \t") - pos);
            std::getline(confFile, line);
            locationstatus = true;
            while (locationstatus)
            {
                currentLocation += trim(line) + "\n";
                std::getline(confFile, line);
                if (line.find("}") != std::string::npos)
                    locationstatus = false;
            }
            routes.insert(std::make_pair(routePath, new Location(currentLocation)));
            continue;
        }
    }
    confFile.close();
}

void Conf::setConf(const std::string& key, const std::string value)
{
    configMap[key] = value; 
}

/**
 * @brief Checkers
 */
Location *Conf::checkRoute(std::string const & routePath)
{
    std::map<std::string, Location*>::iterator it = routes.begin();
    
    std::string routePath2 = extractLastSegment(routePath);
    for (;it != routes.end(); ++it)
        if (routePath2 == it->first)
            return it->second;
    return NULL;    
}

void Conf::checkAndSetDefaultValues()
{
    if (listenPorts.empty())
        listenPorts.push_back("8080");
    std::map<std::string, std::string>::iterator it;
    for (it = configMap.begin(); it != configMap.end(); ++it) 
    {
        if (it->second.empty()) 
        {   
            if (it->first == "listen") 
                it->second = "8080"; 
            else if (it->first == "host") 
                it->second = "127.0.0.1"; 
            else if (it->first == "server_name") 
                it->second = "myserver.local"; 
            else if (it->first == "error_page_403") 
                it->second = "/default/error_pages/403.html";
            else if (it->first == "error_page_404")
                it->second = "/default/error_pages/404.html";
            else if (it->first == "error_page_405") 
                it->second = "/default/error_pages/405.html";
            else if (it->first == "error_page_413") 
                it->second = "/default/error_pages/413.html";
            else if (it->first == "error_page_500") 
                it->second = "/error_pages/500.html"; 
            else if (it->first == "client_max_body_size") 
                it->second = "4M";
            else if (it->first == "keepalive_timeout")
                it->second = "65";
            else if (it->first == "client_body_timeout")
                it->second = "60";
            else if (it->first == "client_header_timeout")
                it->second = "10"; 
            else if (it->first == "location_/root")
                it->second = "/www/html";
            else if (it->first == "location_/index")
                it->second = "index.html";
            else if (it->first == "location_/methods")
                it->second = "GET POST";
            else if (it->first == "location_/images/root") 
                it->second = "/www/images";
            else if (it->first == "location_/images/autoindex")
                it->second = "on";
            else if (it->first == "location_/upload/root")
                it->second = "/www/uploads";
            else if (it->first == "location_/upload/methods")
                it->second = "POST";
            else if (it->first == "location_/upload/upload_store")
                it->second = "/uploads/";
            else if (it->first == "location_/cgi-bin/root")
                it->second = "/www/cgi-bin";
            else if (it->first == "location_/cgi-bin/cgi")
                it->second = "on";
            else if (it->first == "location_/cgi-bin/cgi_bin")
                it->second = "/cgi-bin/";
            else if (it->first == "location_/cgi-bin/methods")
                it->second = "GET POST";
            else if (it->first == "location_/cgi-bin/extension")
                it->second = ".php";
            else if (it->first == "location_/old-page/return")
                it->second = "301 /new-page";
        }
    }
}

/**
 * @brief Init functions
 */
void Conf::init()
{
    printStatus(getConfig("location_/cgi-bin/cgi") == "on", "enable Cgi");
}

void Conf::debugFile()
{
}

/**
 * @brief Check if key exists
 */
bool Conf::hasKey(std::string const& key) const
{
    return configMap.find(key) != configMap.end();
}
