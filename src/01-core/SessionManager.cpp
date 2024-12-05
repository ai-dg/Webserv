/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SessionManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:18 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/05 20:41:14 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/SessionManager.hpp"
#include "../00-headers/02-utils/Log.hpp"

std::string const SESSION_FILE_PATH = "./sessions/session_data.txt";

/**
 * @brief Private methods
 */
std::string SessionManager::generateSessionsId() 
{
    std::stringstream ss;
    for (int i = 0; i < 16; ++i) 
    {
        int randomValue = std::rand() % 16;
        ss << std::hex << randomValue;
    }
    std::string sessionId = ss.str();
    return sessionId;
}

/**
 * @brief Copelien form
 */
SessionManager::SessionManager() 
{
    std::srand(std::time(0));
    loadSessionsFromFile();
}

SessionManager::SessionManager(SessionManager const& src) 
{
    *this = src;
}

SessionManager& SessionManager::operator=(SessionManager const& src) 
{
    if (this != &src) 
    {
        sessions = src.sessions;
    }
    return *this;
}

SessionManager::~SessionManager() 
{
    Log::cleanup();
}

/**
 * @brief Public methods
 */
std::string SessionManager::createSessions() 
{
    std::string sessionId = generateSessionsId();
    sessions[sessionId] = std::map<std::string, std::string>();
    return sessionId;
}

bool SessionManager::sessionExist(std::string const& sessionId) 
{
    bool exists = sessions.find(sessionId) != sessions.end();
    return exists;
}

std::map<std::string, std::string>& SessionManager::getSession(std::string const& sessionId) 
{
    if (sessions.find(sessionId) == sessions.end()) 
        sessions[sessionId] = std::map<std::string, std::string>();
    return sessions[sessionId];
}

void SessionManager::saveSessionsToFile()
{
    std::ofstream file(SESSION_FILE_PATH.c_str());
    if (!file.is_open()) 
        return;
    for (std::map<std::string, std::map<std::string, std::string> >::iterator it = sessions.begin(); it != sessions.end(); ++it) 
    {
        file << it->first << "\n";
        for (std::map<std::string, std::string>::iterator data_it = it->second.begin(); data_it != it->second.end(); ++data_it) 
            file << data_it->first << "=" << data_it->second << "\n";
        file << "---\n";
    }
    file.close();
}

void SessionManager::loadSessionsFromFile()
{
    std::ifstream file(SESSION_FILE_PATH.c_str());
    if (!file.is_open()) 
        return;

    std::string line, sessionId;
    while (std::getline(file, line)) 
    {
        if (line == "---") 
        {
            sessionId = "";
            continue;
        }
        if (sessionId.empty()) 
        {
            sessionId = line;
            sessions[sessionId] = std::map<std::string, std::string>();
        } 
        else 
        {
            size_t pos = line.find('=');
            if (pos != std::string::npos) 
            {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                sessions[sessionId][key] = value;
            }
        }
    }
    file.close();
}
