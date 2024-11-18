#ifndef CONF_HPP
#define CONF_HPP

#include <map>
#include <string>
#include <vector>
#include <stdexcept>

class Conf
{
    private:
        std::map<std::string, std::string> configMap;
        std::string path;
        std::vector<std::string> listenPorts;

    public:
        Conf();
        void operator=(Conf &conf);
        Conf(std::string& path);
        ~Conf();
        
        bool checkFormatOfConfig();
        void getValuesFromPath();
        void init();
        void printStatus(bool status, std::string text);
        void setConf(std::string const& key, std::string const value);
        std::string getConfig(const std::string& key) const;
        bool hasKey(std::string const& key) const;
        void printConfigs(std::ofstream& out) const;
        void checkAndSetDefaultValues();
        const std::vector<std::string>& getListenPorts() const;

    class ConfNotCorrectFormat : public std::exception
    {
        public:
            virtual const char* what() const throw();
    };

};


#endif