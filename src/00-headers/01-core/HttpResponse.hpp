/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:55:53 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 19:32:15 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../00-shared/includes.hpp"
#include "../01-core/Server.hpp"
#include "../01-core/HttpRequest.hpp"
#include "../02-utils/files.hpp"
#include "../02-utils/stringUtils.hpp"
#include "../02-utils/date.hpp"

class HttpResponse
{
    private :
        std::map<std::string, std::string> headers;
        std::string mimeType;
        std::string filePath;
        std::string body;
        int statusCode;

        /**
         * @brief Private setters
         */
        void setMineType(void);
        
    public :
        /**
         * @brief Copelin form
         */
        HttpResponse(const HttpRequest &req);
        HttpResponse(const HttpResponse &src);
        HttpResponse &operator=(const HttpResponse &src);
        ~HttpResponse();

        /**
         * @brief Setters
         */
        void setRedirection(std::string newPath, int status);
        void setRedirection(std::string newPath);
        void setRedirection(int status);
        void setResourcePath(const HttpRequest &req);
        void setStatusCode(int stat);
        void setBody(std::string);

        /**
         * @brief Getters
         */
        std::string getHeaders();
        std::string getFilePath() const;
        
        /**
         * @brief Public methods
         */
        void addHeader(const std::string &key, const std::string &value);
        void checkRedirection(const HttpRequest &req);
        void send(int fd_client);

};
