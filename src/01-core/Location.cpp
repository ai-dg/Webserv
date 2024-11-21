/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:08 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 19:55:19 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/Location.hpp"
#include "../00-headers/02-utils/stringUtils.hpp"
#include "../00-headers/02-utils/directories.hpp"
#include "../00-headers/02-utils/Log.hpp"

/**
 * @brief Private functions
 */
void Location::extractField(std::string param)
{
    std::string field;
    std::string value;

    value = param.substr(param.find_first_of(" \t") + 1, param.find_last_of(";") - param.find_first_of(" \t") - 1);
    field = param.substr(0, param.find_first_of(" \t"));
    if (field == "root")
        _root = value;
    if (field == "index")
    {
        _index = value;
        setAllowedIndexes();
    }
    if (field == "extensions")
        _extensions = value;
    if (field == "methods")
        _methods = value;
    if (field == "autoindex")
        _autoindex = value;
    if (field == "upload_store")
        _upload_store = value;
    if (field == "cgi_bin")
        _cgi_bin = value;
    if (field == "cgi")
        _cgi = value;
    if (field == "return")
    {
        _return = value;
        setRedirectionStatus();
        setRedirectionPath();
    }
}

/**
 * @brief Copelien form
 */
Location::Location()
{
    init();
    Log::output("./sessions/Location.txt") << "Location class object created" << std::endl;
}

Location::Location(std::string &loc)
{
    std::string currentLine;
    init();
    while(loc.find("\n") != std::string::npos)
    {
        currentLine = loc.substr(0, loc.find_first_of("\n"));
        extractField(currentLine);
        loc.erase(0,loc.find_first_of("\n") + 1);
    }
}

Location::Location(Location const& src) : _root(src._root), _extensions(src._extensions), _methods(src._methods), _autoindex(src._autoindex), _upload_store(src._upload_store), _cgi(src._cgi), _cgi_bin(src._cgi_bin), _return(src._return), _index(src._index), indexes(src.indexes), redirectionPath(src.redirectionPath), redirectionStatus(src.redirectionStatus)
{
    Log::output("./sessions/Location.txt") << "Location class object copied" << std::endl;
}

Location& Location::operator=(Location const& src)
{
    if (this == &src)
        return *this;
    _root = src._root;
    _extensions = src._extensions;
    _methods = src._methods;
    _autoindex = src._autoindex;
    _upload_store = src._upload_store;
    _cgi = src._cgi;
    _cgi_bin = src._cgi_bin;
    _return = src._return;
    _index = src._index;
    indexes = src.indexes;
    redirectionPath = src.redirectionPath;
    redirectionStatus = src.redirectionStatus;
    Log::output("./sessions/Location.txt") << "Location class object assigned" << std::endl;
    return *this;
}

Location::~Location()
{
    Log::output("./sessions/Location.txt") << "Location class object destroyed" << std::endl;
    Log::cleanup();
}

/**
 * @brief Init
 */
void Location::init()
{
    _root = "";
    _extensions = "";
    _methods = "";
    _autoindex = "";
    _upload_store = "";
    _cgi = "";
    _cgi_bin = "";
    _return = "";
    _index = "";
    redirectionPath = "";
    redirectionStatus = 0;
}

/**
 * @brief Setters
 */
void Location::setRedirectionStatus()
{
    redirectionStatus = stoi(_return.substr(0,_return.find(" ")).c_str());
}

void Location::setAllowedIndexes()
{
    std::string cpy = trim(_index);
    if (_index == "")
        return ;
    size_t pos = cpy.find(" \t");
    if (pos == std::string::npos && cpy.size() > 0)
    {
        indexes.push_back(cpy);
        return ;
    }
    while (pos != std::string::npos)
    {       
        indexes.push_back(cpy.substr(0, pos));
        cpy.erase(0, pos + 1);
        pos = cpy.find(" \t");
        if (pos == std::string::npos && cpy.size() > 0)
            indexes.push_back(cpy);
    }
}

void Location::setRedirectionPath()
{
    redirectionPath = _return.substr(_return.find(" ") + 1, std::string::npos);
}

/**
 * @brief Getters
 */
int Location::getRedirectionStatus()
{
    return redirectionStatus;
}

std::string Location::getRedirectionPath()
{
    return redirectionPath;
}

/**
 * @brief Public functions
 */
std::string Location::index()
{
    return _index;
}
std::string Location::redirection()
{
    return _return;
}
std::string Location::cgi()
{
    return _cgi;
}
std::string Location::cgi_bin()
{
    return _cgi_bin;
}
std::string Location::upload_store()
{
    return _upload_store;
}
std::string Location::autoindex()
{
    return _autoindex;
}
std::string Location::methods()
{
    return _methods;
}
std::string Location::extensions()
{
    return _extensions;
}
std::string Location::root()
{
    return _root;
}
std::string Location::findIndex()
{
    std::vector<std::string>::iterator it;
    it = indexes.begin();
    for (; it != indexes.end(); ++it)
    {
        if (doesFileExist("./" +_root + "/" + *it))
            return *it;
    }
    return "";
}

/**
 * @brief Debug
 */
void Location::debugValues()
{
    if (_root != "")
        Log::output("./sessions/Location.txt") << "\t\t" << BOLD_WHITE << _root << RESET << std::endl;
    if (_extensions != "")
        Log::output("./sessions/Location.txt") << "\t\t" << BOLD_WHITE << _extensions << RESET << std::endl;
    if (_methods != "")
        Log::output("./sessions/Location.txt") << "\t\t" << BOLD_WHITE << _methods << RESET << std::endl;
    if (_autoindex != "")
        Log::output("./sessions/Location.txt") << "\t\t" << BOLD_WHITE << _autoindex << RESET << std::endl;
    if (_upload_store != "")
        Log::output("./sessions/Location.txt") << "\t\t" << BOLD_WHITE << _upload_store << RESET << std::endl;
    if (_cgi_bin != "")
        Log::output("./sessions/Location.txt") << "\t\t" << BOLD_WHITE << _cgi_bin << RESET << std::endl;
    if (_cgi != "")
        Log::output("./sessions/Location.txt") << "\t\t" << BOLD_WHITE << _cgi << RESET << std::endl;
    if (_return != "")
        Log::output("./sessions/Location.txt") << "\t\t" << BOLD_WHITE << _return << RESET << std::endl;
    if (_index != "")
        Log::output("./sessions/Location.txt") << "\t\t" << BOLD_WHITE << _index << RESET << std::endl;
}
