#include "../headers/Conf.hpp"
#include <iostream>
#include <fstream>

Conf::Conf(std::string& path) : path(path)
{
    std::cout << "Initialize conf path" << std::endl;

    
    configMap.insert(std::make_pair("listen", ""));
    configMap.insert(std::make_pair("host", ""));
    configMap.insert(std::make_pair("server_name", ""));
    configMap.insert(std::make_pair("error_page_404", ""));
    configMap.insert(std::make_pair("error_page_500", ""));
    configMap.insert(std::make_pair("client_max_body_size", ""));

    
    configMap.insert(std::make_pair("location_/root", ""));
    configMap.insert(std::make_pair("location_/index", ""));
    configMap.insert(std::make_pair("location_/methods", ""));

    
    configMap.insert(std::make_pair("location_/images/root", ""));
    configMap.insert(std::make_pair("location_/images/autoindex", ""));

    
    configMap.insert(std::make_pair("location_/upload/root", ""));
    configMap.insert(std::make_pair("location_/upload/methods", ""));
    configMap.insert(std::make_pair("location_/upload/upload_store", ""));

    
    configMap.insert(std::make_pair("location_/cgi-bin/root", ""));
    configMap.insert(std::make_pair("location_/cgi-bin/cgi", ""));
    configMap.insert(std::make_pair("location_/cgi-bin/cgi_bin", ""));
    configMap.insert(std::make_pair("location_/cgi-bin/methods", ""));
    configMap.insert(std::make_pair("location_/cgi-bin/extension", ""));

    
    configMap.insert(std::make_pair("location_/old-page/return", ""));
}



Conf::~Conf()
{
    std::cout << "Conf malloc destroyed" << std::endl;

}

void Conf::getValuesFromPath()
{
    std::ifstream confFile(path.c_str());
    std::string line;
    std::string currentLocation = ""; 

    if (!confFile.is_open()) {
        std::cerr << "Unable to open configuration file: " << path << std::endl;
        return;
    }

    while (std::getline(confFile, line)) {
        
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        
        if (line.empty() || line[0] == '#') continue;

        
        if (line.find("location") == 0) {
            size_t pos = line.find(" ");
            currentLocation = line.substr(pos + 1); 
            currentLocation = currentLocation.substr(0, currentLocation.find("{")); 
            currentLocation.erase(0, currentLocation.find_first_not_of(" \t")); 
            currentLocation.erase(currentLocation.find_last_not_of(" \t") + 1); 
            continue;
        }

        
        if (line == "}") {
            currentLocation = ""; 
            continue;
        }

        
        size_t pos = line.find(' ');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            
            if (!value.empty() && value[value.length() - 1] == ';') {
                value.erase(value.length() - 1); 
            }

            
            if (!currentLocation.empty()) {
                
                if (currentLocation[currentLocation.length() - 1] != '/') {
                    key = "location_" + currentLocation + "/" + key;
                } else {
                    key = "location_" + currentLocation + key;
                }

                
                key.erase(0, key.find_first_not_of(" \t")); 
                key.erase(key.find_last_not_of(" \t") + 1); 
            }

            
            std::cout << "Key: " << key << ", Value: " << value << std::endl;

            
            if (hasKey(key)) {
                setConf(key, value);
            } else {
                std::cout << "Key not found: " << key << std::endl; 
            }
        }
    }
}






void Conf::setConf(const std::string& key, const std::string value)
{
    configMap[key] = value; 
}

std::string Conf::getConfig(const std::string& key) const
{
    
    std::map<std::string, std::string>::const_iterator it = configMap.find(key);
    if (it != configMap.end()) {
        return it->second;
    }
    return ""; 
}


bool Conf::hasKey(std::string const& key) const
{
    return configMap.find(key) != configMap.end();
}


void Conf::printConfigs() const
{
    
    std::map<std::string, std::string>::const_iterator it;
    std::cout << "-------Config values from map---------" << std::endl;
    for (it = configMap.begin(); it != configMap.end(); ++it) {
        std::cout << it->first << ": ";
        if (it->second.empty()) {
            std::cout << "NULL";
        } else {
            std::cout << it->second;
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------------------------" << std::endl;
}


void Conf::checkAndSetDefaultValues()
{
    
    std::map<std::string, std::string>::iterator it;
    for (it = configMap.begin(); it != configMap.end(); ++it) {
        if (it->second.empty()) {  
            
            if (it->first == "listen") {
                it->second = "8090"; 
            } else if (it->first == "host") {
                it->second = "127.0.0.1"; 
            } else if (it->first == "server_name") {
                it->second = "myserver.local"; 
            } else if (it->first == "error_page_404") {
                it->second = "/error_pages/404.html"; 
            } else if (it->first == "error_page_500") {
                it->second = "/error_pages/500.html"; 
            } else if (it->first == "client_max_body_size") {
                it->second = "4M"; 
            } 
            
            else if (it->first == "location_/root") {
                it->second = "/www/html"; 
            } else if (it->first == "location_/index") {
                it->second = "index.html"; 
            } else if (it->first == "location_/methods") {
                it->second = "GET POST"; 
            } else if (it->first == "location_/images/root") {
                it->second = "/www/images"; 
            } else if (it->first == "location_/images/autoindex") {
                it->second = "on"; 
            } else if (it->first == "location_/upload/root") {
                it->second = "/www/uploads"; 
            } else if (it->first == "location_/upload/methods") {
                it->second = "POST"; 
            } else if (it->first == "location_/upload/upload_store") {
                it->second = "/uploads/"; 
            } else if (it->first == "location_/cgi-bin/root") {
                it->second = "/www/cgi-bin"; 
            } else if (it->first == "location_/cgi-bin/cgi") {
                it->second = "on"; 
            } else if (it->first == "location_/cgi-bin/cgi_bin") {
                it->second = "/cgi-bin/"; 
            } else if (it->first == "location_/cgi-bin/methods") {
                it->second = "GET POST"; 
            } else if (it->first == "location_/cgi-bin/extension") {
                it->second = ".php"; 
            } else if (it->first == "location_/old-page/return") {
                it->second = "301 /new-page"; 
            }
        }
    }
}
