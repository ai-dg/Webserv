/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:55:59 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/04 23:33:35 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../00-shared/includes.hpp"

class Location
{
    private:
        std::string _root;
        std::string _extensions ;
        std::string _methods ;
        std::string _autoindex ;
        std::string _upload_store ;
        size_t _max_body ;
        std::string _cgi;
        std::string _cgi_bin;
        std::string _exe;
        std::string _return;
        std::string _index;
        std::vector<std::string> indexes;
        std::string redirectionPath;
        int redirectionStatus;

        /**
         * @brief Private functions
         */
        void extractField(std::string field);
    
    public:
        /**
         * @brief Copelien form
         */
        Location();
        Location(Location const& src);
        Location& operator=(Location const& src);
        Location(std::string &loc);
        ~Location();

        /**
         * @brief Init
         */
        void init();

        /**
         * @brief Setters
         */
        void setRedirectionStatus();
        void setAllowedIndexes();
        void setRedirectionPath();
        void setMaxBodySize(std::string size);

        /**
         * @brief Getters
         */
        int getRedirectionStatus();  
        std::string getRedirectionPath();

        /**
         * @brief Public functions
         */
        std::string index();
        std::string redirection();
        std::string cgi();
        std::string exe();
        std::string cgi_bin();
        std::string upload_store();
        std::string autoindex();
        std::string methods();
        std::string extensions();
        std::string root();
        std::string findIndex();
        size_t max_body_size();

        /**
         * @brief Debug
         */
        void debugValues();
};
