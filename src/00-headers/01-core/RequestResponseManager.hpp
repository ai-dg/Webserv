#pragma once

#include "../00-shared/includes.hpp"
#include "../00-shared/defines.hpp"
#include "../01-core/Server.hpp"
#include "../01-core/SessionManager.hpp"
#include "../01-core/RarManager.hpp"
#include "../01-core/SignalHandler.hpp"
#include "../02-utils/Log.hpp"
#include "../01-core/Epoll.hpp"


class RequestResponseManager 
{
    private:
        std::vector<int>& m_fdSockets;
        std::vector<Server*>& m_servers;
        SessionManager& m_sessionManager;
        
        Epoll m_epoll;
        bool m_acceptPaused;
        int fdLimit;
        std::map<int, std::string> m_requestMap;

        void initializeEpoll();
        bool handleNewConnection(const struct epoll_event& event);
        bool processRequest(int fdClient);
        bool parseAndProcessRequest(int fdClient, std::string& currentRequest);
        bool handleChunkedRequest(int fdClient, std::string& currentRequest);
        bool handleContentLengthRequest(int fdClient, std::string& currentRequest);
        void processRequestWithServer(int fdClient, std::string& request);
        void handleClientDisconnection(int fdClient);
        void handleRecvError(int fdClient);
        bool isSocketReadyForRead(int fd_Client);
        int findFdLimit();

    public:
        RequestResponseManager
        (
            std::vector<int>& fdSockets, 
            std::vector<Server*>& servers, 
            SessionManager& sessionManager
        );
        void run();
};