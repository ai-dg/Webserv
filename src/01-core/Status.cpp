/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:31 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 20:24:38 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/Status.hpp"
#include "../00-headers/02-utils/stringUtils.hpp"

// Initialisation de la map statique
const std::map<int, std::string> Status::codes = Status::initializeCodes();

std::map<int, std::string> Status::initializeCodes() 
{
    std::map<int, std::string> temp_codes;
    temp_codes[200] = " OK";
    temp_codes[201] = " Created";
    temp_codes[204] = " No Content";
    temp_codes[301] = " Moved Permanently";
    temp_codes[302] = " Found";
    temp_codes[304] = " Not Modified";
    temp_codes[400] = " Bad Request";
    temp_codes[401] = " Unauthorized";
    temp_codes[403] = " Forbidden";
    temp_codes[404] = " Not Found";
    temp_codes[405] = " Method Not Allowed";
    temp_codes[406] = " Not Acceptable";
    temp_codes[408] = " Request Timeout";
    temp_codes[409] = " Conflict";
    temp_codes[410] = " Gone";
    temp_codes[411] = " Length Required";
    temp_codes[413] = " Payload Too Large";
    temp_codes[414] = " URI Too Long";
    temp_codes[415] = " Unsupported Media Type";
    temp_codes[429] = " Too Many Requests";
    temp_codes[500] = " Internal Server Error";
    temp_codes[501] = " Not Implemented";
    temp_codes[502] = " Bad Gateway";
    temp_codes[503] = " Service Unavailable";
    temp_codes[504] = " Gateway Timeout";
    temp_codes[505] = " HTTP Version Not Supported";
    temp_codes[507] = " Insufficient Storage";
    return temp_codes;
}

std::string Status::get(int code)
{
    std::map<int, std::string>::const_iterator it = Status::codes.find(code);
    if (it != Status::codes.end())
        return it->second;
    return "Unknown Status Code";
}

std::string Status::get(std::string code)
{
    int iCode = stoi(code);
    std::map<int, std::string>::const_iterator it = Status::codes.find(iCode);
    if (it != Status::codes.end())
        return it->second;
    return "Unknown Status Code";
}
