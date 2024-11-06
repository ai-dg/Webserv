#ifndef CONF_HPP
#define CONF_HPP

#include <map>
#include <string>
#include <vector>
#include "../headers/Location.hpp"

class Conf
{
    private:
        std::map<std::string, std::string> configMap;
        std::map<std::string, Location*> routes;
        std::string path;
        std::vector<std::string> listenPorts;

    public:
        Conf();
        void operator=(Conf &conf);
        Conf(std::string& path);
        ~Conf();

        void getValuesFromPath();
        void init();
        void printStatus(bool status, std::string text);
        void setConf(std::string const& key, std::string const value);
        std::string getConfig(const std::string& key) const;
        bool hasKey(std::string const& key) const;
        void printConfigs(std::ofstream& out) const;
        void checkAndSetDefaultValues();
        const std::vector<std::string>& getListenPorts() const;
};


#endif