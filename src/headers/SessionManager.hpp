#ifndef SESSIONMANAGER_HPP
#define SESSIONMANAGER_HPP

#include <map>
#include <string>

class SessionManager
{
    private:
        std::map<std::string, std::map<std::string, std::string> > sessions;
        std::string generateSessionsId();

    public:
        SessionManager();
        ~SessionManager();
        std::string createSessions();
        bool sessionExist(std::string const& sessionId);
        std::map<std::string, std::string>& getSession(std::string const& sessionId);
};

#endif
