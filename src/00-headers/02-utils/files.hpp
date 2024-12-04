/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:39:51 by ls                #+#    #+#             */
/*   Updated: 2024/12/05 00:27:50 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../00-shared/includes.hpp"

std::string checkMimeType(const std::string& path);
std::string getMime(const std::string& mime);
std::string getFile(const std::string& path);
bool checkFormatOfConfig(std::string const& path_file);
bool checkFormatOfPaths(std::string const& path_file);
char *resolvePath(std::string path);

class PathNotCorrectFormat : public std::exception
{
    public:
        virtual const char* what() const throw();
};

bool hasExtension(std::string file);
bool isValidUri(std::string uri);
