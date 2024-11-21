/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Conf.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:55:35 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 18:55:36 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../00-shared/includes.hpp"
#include "../01-core/Location.hpp"

class Conf
{
    private:
        std::map<std::string, std::string> configMap;
        std::map<std::string, Location*> routes;
        std::string path;
        std::vector<std::string> listenPorts;

    public:
        /**
         * @brief Coplien Form
         */
        Conf(std::string& path);
        Conf(Conf const& src);
        Conf& operator=(Conf &conf);
        ~Conf();

        /**
         * @brief Print functions
         */
        void printRoutesConfig(int servNb);
        void printStatus(bool status, std::string text);
        void printConfigs(std::ofstream& out) const;
        void printFile();

        /**
         * @brief Getters
         */
        void getValuesFromPath();
        std::string getConfig(const std::string& key) const;
        const std::vector<std::string>& getListenPorts() const;

        /**
         * @brief Setters
         */
        void setLocations();
        void setConf(std::string const& key, std::string const value);

        /**
         * @brief Checkers
         */
        Location *checkRoute(std::string const & route);
        void checkAndSetDefaultValues();

        /**
         * @brief Init
         */
        void init();

        /**
         * @brief Debug
         */
        void debugFile();

        /**
         * @brief Check if key exists
         */
        bool hasKey(std::string const& key) const;
};
