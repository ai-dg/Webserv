/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:55:50 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/16 10:21:07 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../00-shared/includes.hpp"
#include "../01-core/Server.hpp"
#include "../02-utils/format.hpp"

class HttpRequest
{
    private:
        std::map<std::string, std::string> headers;
        std::string method;
        std::string host;
        std::string URI;
        std::string route;
        std::string askedFile;
        std::string body;
        Server *server;

        /**
         * @brief Private setters
         */
        
        void setMethod(std::string req);
        void setHeaders(std::string req);
        void setBody(std::string req);
        void setRoute();
        void setAskedFile();
        void setURI(std::string req);
        std::string mergeChunks(std::string data);

        /**
         * @brief Private parsers
         */
        void parseRequest(std::string req);
        void addToHeaders(std::string line);
        
    public:
        /**
         * @brief Copelin form
         */
        HttpRequest();
        HttpRequest(std::string req);
        HttpRequest(std::string req, std::vector<Server *> Servers);
        HttpRequest(std::string req, Server *server);
        HttpRequest(HttpRequest const& src);
        HttpRequest &operator=(HttpRequest const&src);
        ~HttpRequest();
     
        /**
         * @brief Getters
         */
        std::string getRoute() const;
        std::string getRequestedFile() const;
        std::string getQueryString() const;
        std::string getURI() const;
        std::string getAskedFile() const;
        std::string getConf(std::string key) const;
        std::string getHeader(std::string key) const;
        std::string getFormatedHeader(std::string key);
        std::string getMethod() const;
        std::string getBody() const; 
        void getHostByName() const;
        Location *getRouteConf(std::string const & route) const;
        std::map<std::string, std::string> getHeaders() const;

        /**
         * @brief Validators
         */
        bool hasFileSpecialRoute(std::string filePath) const;
        bool isScript() const;
        bool isStatic() const;
        bool isValidBodySize() const;

        /**
         * @brief Printers
         */
        void printConf(std::string) const;
};
