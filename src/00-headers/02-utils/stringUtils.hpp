/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stringUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:57:30 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/05 01:51:28 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../00-shared/includes.hpp"

std::string getExtension(std::string filePath);
std::string replaceBy(std::string original, std::string find, std::string replace);
std::pair<std::string, std::string> split(std::string const& str, char delimiter);
std::string itos(int number);
int stoi(std::string str);
std::string trim(std::string str);
std::string upperCaseMe(std::string);
std::string upperCaseMe(std::string);
std::string extractLastSegment(const std::string& path);
std::string removeDuplicateSlashes(const std::string& path);
size_t getFormatedSizeFromString(std::string size);
size_t getNextof(std::string str, size_t pos, char c);
std::string cleanString(const std::string& str);
