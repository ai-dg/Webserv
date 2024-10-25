/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:39:51 by ls                #+#    #+#             */
/*   Updated: 2024/10/24 13:12:55 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef FILES_HPP
#define FILES_HPP

#include "includes.hpp"
#include <string>

std::string checkMimeType(const std::string& path);
std::string getMime(const std::string& mime);
std::string getFile(const std::string& path);

#endif