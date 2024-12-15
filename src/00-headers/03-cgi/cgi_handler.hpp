/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:57:33 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/05 04:40:47 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../00-shared/includes.hpp"
#include "../01-core/HttpRequest.hpp"
#include "../01-core/HttpResponse.hpp"
#include "../01-core/Location.hpp"
#include "../01-core/Pipe.hpp"
#include "../02-utils/stringUtils.hpp"

class Cgi_handler
{
    protected:
        std::string scriptPath;
        std::string queryString;
        int fd_client;
        HttpRequest *req;
        HttpResponse *res;
        Location *route;
        std::vector<char *> environment;

        /**
         * @brief Prepare the environment for the CGI script
         */     
        std::string getExeContext(std::string file);
        void addToEnvironment(std::string env);
        void addToEnvironment(const char * env);
        void setEnvironment();
        void debugEnvironment();
        void parentCgiProcess(Pipe &pipe_in, Pipe &pipe_out, pid_t pid);
        void childCgiProcess(Pipe &pipe_in, Pipe &pipe_out);        
        void setupSpecialRoute(std::string &path, std::string &scriptPathTemp, std::string &exe_context);
        void executeScript(std::string &path, std::string &scriptPathTemp, std::string &exe_context);
        int handleErrorStatus(int status);
        void handleDirectIO(Pipe &pipe_out);
        void handleIOWithHeaders(Pipe &pipe_out);
        std::string getHeaders(std::string const &status);
        void sendHeaders(std::string const &headersStr);
        void sendBody(std::string &remainingBuffer, std::string &bufferAccumulator);
 
    public:
        /**
         * @brief Coplien form
         */
        Cgi_handler();
        Cgi_handler(int fd_client, HttpRequest *req, HttpResponse *res, Location *route);
        Cgi_handler(Cgi_handler const& src);
        Cgi_handler& operator=(Cgi_handler const& src);
        ~Cgi_handler();

        /**
         * @brief Execute the CGI script
         */
        void executeCGI(std::string const& scriptPath);
};
