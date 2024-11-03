#include "../headers/SessionManager.hpp"
#include "../headers/Log.hpp"
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iostream>
#include <fstream>

std::string const SESSION_FILE_PATH = "./sessions/session_data.txt";

/**
 * @brief Public:
 */
SessionManager::SessionManager() 
{
    std::srand(std::time(0));
    loadSessionsFromFile();
    Log::output("./sessions/SessionManager.txt") << "SessionManager created" << std::endl;
}

SessionManager::~SessionManager() 
{
    Log::output("./sessions/SessionManager.txt") << "SessionManager destroyed" << std::endl;
}

std::string SessionManager::createSessions() 
{
    std::string sessionId = generateSessionsId();
    sessions[sessionId] = std::map<std::string, std::string>();
    // Log::output("./sessions/SessionManager.txt") << "New session created with ID: " << sessionId << " and added to session map." << std::endl;

    if (sessions.find(sessionId) != sessions.end()) 
    {
        Log::output("./sessions/SessionManager.txt") << "Session successfully added to map." << std::endl;
    } 
    else 
    {
        Log::output("./sessions/SessionManager.txt") << "Error: Session was not added to map." << std::endl;
    }
    
    return sessionId;
}

bool SessionManager::sessionExist(std::string const& sessionId) 
{
    bool exists = sessions.find(sessionId) != sessions.end();
    // Log::output("./sessions/SessionManager.txt") << "Session exists check for ID " << sessionId << ": ";
    if (exists)
    {
        Log::output("./sessions/SessionManager.txt") << "Yes" << std::endl;
    } 
    else 
    {
        Log::output("./sessions/SessionManager.txt") << "No" << std::endl;
        // Log::output("./sessions/SessionManager.txt") << "Reason: session ID " << sessionId << " not found in session map." << std::endl;
    }
    return exists;
}

std::map<std::string, std::string>& SessionManager::getSession(std::string const& sessionId) 
{
    if (sessions.find(sessionId) == sessions.end()) 
    {
        // Log::output("./sessions/SessionManager.txt") << "Creating empty session data for session ID: " << sessionId << std::endl;
        sessions[sessionId] = std::map<std::string, std::string>();
    } 
    else 
    {
        Log::output("./sessions/SessionManager.txt") << "Session data found for session ID: " << sessionId << std::endl;
    }
    return sessions[sessionId];
}

/**
 * @brief Private:
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
    // Log::output("./sessions/SessionManager.txt") << "Generated session ID: " << sessionId << std::endl;
    return sessionId;
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
        {
            file << data_it->first << "=" << data_it->second << "\n";
        }
        file << "---\n";
    }
    file.close();
    // Log::output("./sessions/SessionManager.txt") << "Sessions saved to file." << std::endl;
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
    // Log::output("./sessions/SessionManager.txt") << "Sessions loaded from file." << std::endl;
}
