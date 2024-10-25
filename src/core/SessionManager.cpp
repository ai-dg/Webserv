#include "../headers/SessionManager.hpp"
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iostream>

/**
 * @brief Public:
 */
SessionManager::SessionManager() 
{
    std::srand(std::time(0));
    std::cout << "SessionManager Created" << std::endl;
}

SessionManager::~SessionManager() 
{
    std::cout << "SessionManager destroyed" << std::endl;
}

std::string SessionManager::createSessions() 
{
    std::string sessionId = generateSessionsId();
    sessions[sessionId] = std::map<std::string, std::string>();
    return sessionId;
}

bool SessionManager::sessionExist(std::string const& sessionId) 
{
    return sessions.find(sessionId) != sessions.end();
}

std::map<std::string, std::string>& SessionManager::getSession(std::string const& sessionId) 
{
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
    return ss.str();
}
