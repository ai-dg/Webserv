/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SessionManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:18 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 20:20:16 by dagudelo         ###   ########.fr       */
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
    Log::output("./sessions/SessionManager.txt") << "Generated session ID: " << sessionId << std::endl;
    return sessionId;
}

/**
 * @brief Copelien form
 */
SessionManager::SessionManager() 
{
    std::srand(std::time(0));
    loadSessionsFromFile();
    Log::output("./sessions/SessionManager.txt") << "SessionManager object class created" << std::endl;
}

SessionManager::SessionManager(SessionManager const& src) 
{
    *this = src;
    Log::output("./sessions/SessionManager.txt") << "SessionManager object class copied" << std::endl;
}

SessionManager& SessionManager::operator=(SessionManager const& src) 
{
    if (this != &src) 
    {
        sessions = src.sessions;
    }
    Log::output("./sessions/SessionManager.txt") << "SessionManager object class assigned" << std::endl;
    return *this;
}

SessionManager::~SessionManager() 
{
    Log::output("./sessions/SessionManager.txt") << "SessionManager object class destroyed" << std::endl;
    Log::cleanup();
}

/**
 * @brief Public methods
 */
std::string SessionManager::createSessions() 
{
    std::string sessionId = generateSessionsId();
    sessions[sessionId] = std::map<std::string, std::string>();
    if (sessions.find(sessionId) != sessions.end()) 
        Log::output("./sessions/SessionManager.txt") << "Session successfully added to map." << std::endl;
    else 
        Log::output("./sessions/SessionManager.txt") << "Error: Session was not added to map." << std::endl;
    return sessionId;
}

bool SessionManager::sessionExist(std::string const& sessionId) 
{
    bool exists = sessions.find(sessionId) != sessions.end();
    if (exists)
        Log::output("./sessions/SessionManager.txt") << "Yes" << std::endl;
    else 
        Log::output("./sessions/SessionManager.txt") << "No" << std::endl;
    return exists;
}

std::map<std::string, std::string>& SessionManager::getSession(std::string const& sessionId) 
{
    if (sessions.find(sessionId) == sessions.end()) 
        sessions[sessionId] = std::map<std::string, std::string>();
    else 
        Log::output("./sessions/SessionManager.txt") << "Session data found for session ID: " << sessionId << std::endl;
    return sessions[sessionId];
}

void SessionManager::saveSessionsToFile()
{
    std::ofstream file(SESSION_FILE_PATH.c_str());
    if (!file.is_open()) 
    {
        Log::output("./logs/error.log") << "Error opening session file for saving: " << SESSION_FILE_PATH << std::endl;
        return;
    }

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
    {
        Log::output("./logs/error.log") << "No existing session file found: " << SESSION_FILE_PATH << std::endl;
        return;
    }

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
