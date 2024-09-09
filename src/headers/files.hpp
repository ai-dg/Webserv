/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ls <ls@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:39:51 by ls                #+#    #+#             */
/*   Updated: 2024/09/09 19:26:07 by ls               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef FILES_HPP
#define FILES_HPP

#include "includes.hpp"
#include <fstream>

std::string checkMimeType(const std::string& path);
std::string getMime(const std::string& mime);
std::string getFile(const std::string& path);

#endif