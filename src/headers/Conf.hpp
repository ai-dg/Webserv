#ifndef CONF_HPP
#define CONF_HPP

#include <map>
#include <string>

class Conf
{
    protected:
        std::map<std::string, std::string> configMap;
        std::string path;

    public:
        Conf();
        void operator=(Conf &conf);
        Conf(std::string& path);
        ~Conf();

        void getValuesFromPath();

        void setConf(std::string const& key, std::string const value);

        std::string getConfig(const std::string& key) const;

        bool hasKey(std::string const& key) const;

        void printConfigs() const;


        void checkAndSetDefaultValues();

};


#endif