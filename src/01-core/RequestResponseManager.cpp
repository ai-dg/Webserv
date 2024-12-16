#include "../00-headers/01-core/RequestResponseManager.hpp"

void RequestResponseManager::initializeEpoll() 
{
    for (size_t i = 0; i < m_fdSockets.size(); ++i) 
    {
        m_epoll.addFd(m_fdSockets[i], EPOLLIN);
        m_epoll.makeSocketNonBlocking(m_fdSockets[i]);
    }
    m_epoll.addFd(signalPipeFd[0], EPOLLIN);
}

bool RequestResponseManager::handleNewConnection(const struct epoll_event& event) 
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    
    int fd_Client = accept(event.data.fd, 
                            (struct sockaddr*)&client_addr, 
                            &client_addr_len);
    if (fd_Client == -1) 
    {
        Log::print_final_log("Erreur lors de accept", "FD:", strerror(errno));
        return false;
    }
    m_epoll.makeSocketNonBlocking(fd_Client);
    m_epoll.addFd(fd_Client, EPOLLIN);
    return true;
}

bool RequestResponseManager::processRequest(int fd_Client) 
{
    std::string& currentRequest = m_requestMap[fd_Client];
    char buff[BUFFER_SIZE];
    ssize_t reads;

    while ((reads = ::recv(fd_Client, buff, BUFFER_SIZE, 0)) > 0)
    {
        if (reads > 0)
            currentRequest.append(buff, reads);
        if (reads == 0) 
        {
            handleClientDisconnection(fd_Client);
            return false;
        }
        if (reads < 0) 
        {
            handleRecvError(fd_Client);
            return false;
        }
    }
    return parseAndProcessRequest(fd_Client, currentRequest);
}

bool RequestResponseManager::parseAndProcessRequest(int fd_Client, std::string& currentRequest) 
{
    size_t headerEnd = currentRequest.find("\r\n\r\n");
    if (headerEnd == std::string::npos) 
            return false;

    if (currentRequest.find("Transfer-Encoding: chunked") != std::string::npos)
        return handleChunkedRequest(fd_Client, currentRequest);

    return handleContentLengthRequest(fd_Client, currentRequest);
}

bool RequestResponseManager::handleChunkedRequest(int fd_Client, std::string& currentRequest) 
{
    size_t chunk_start = currentRequest.find("\r\n\r\n") + 4;

    while (true) 
    {
        size_t chunk_size_end = currentRequest.find("\r\n", chunk_start);
        if (chunk_size_end == std::string::npos) break;

        std::string chunk_size_str = currentRequest.substr(chunk_start, chunk_size_end - chunk_start);
        size_t chunk_size = std::strtol(chunk_size_str.c_str(), NULL, 16);
        
        if (chunk_size == 0) 
        {
            Log::print_final_log("Chunked request received:", "FD:", fd_Client);
            processRequestWithServer(fd_Client, currentRequest);
            currentRequest.clear();
            return true;
        }
        size_t chunk_data_start = chunk_size_end + 2;
        size_t chunk_data_end = chunk_data_start + chunk_size;

        if (chunk_data_end > currentRequest.size())
            break;
        chunk_start = chunk_data_end + 2; 
    }
    return false;
}

bool RequestResponseManager::handleContentLengthRequest(int fd_Client, std::string& currentRequest) 
{
    size_t headerEnd = currentRequest.find("\r\n\r\n");
    size_t content_length_pos = currentRequest.find("Content-Length:");
    
    if (content_length_pos == std::string::npos) 
    {
        Log::print_final_log("No Content-Length detected:", "FD:", fd_Client);
        processRequestWithServer(fd_Client, currentRequest);
        currentRequest.clear();
        return true;
    }

    size_t content_length_start = content_length_pos + strlen("Content-Length:");
    size_t content_length_end = currentRequest.find("\r\n", content_length_start);
    
    if (content_length_end == std::string::npos) 
        return false;

    std::string content_length_str = currentRequest.substr(content_length_start, content_length_end - content_length_start);
    size_t content_length = std::atoi(content_length_str.c_str());
    size_t body_start = headerEnd + 4;

    if (currentRequest.size() >= body_start + content_length) 
    {
        Log::print_final_log("Content-Length request received:", "FD:", fd_Client);
        processRequestWithServer(fd_Client, currentRequest);
        currentRequest.clear();
        return true;
    }

    return false;
}

void RequestResponseManager::processRequestWithServer(int fd_Client, std::string& request) 
{
    int serverIndex = findServerIndex(request, m_servers);
    m_servers[serverIndex]->addFdClient(fd_Client);
    if (m_epoll.purgeTimeOutFds(*(m_servers[serverIndex]->getConf()), fd_Client, m_servers))
        return;
    type_request_manager(&fd_Client, &request, m_servers[serverIndex], m_sessionManager);
}

void RequestResponseManager::handleClientDisconnection(int fd_Client) 
{
    Log::print_final_log("Connection closed by client", "FD:", fd_Client);
    m_epoll.removeFd(fd_Client);
    m_requestMap.erase(fd_Client);
    for (size_t i = 0; i < m_servers.size(); i++) 
        m_servers[i]->removeFdClient(fd_Client);
}

void RequestResponseManager::handleRecvError(int fd_Client) 
{
    Log::print_final_log("Error in recv", "FD: ", strerror(errno));
    m_epoll.removeFd(fd_Client);
    m_requestMap.erase(fd_Client);
    for (size_t i = 0; i < m_servers.size(); i++) 
        m_servers[i]->removeFdClient(fd_Client);
}


RequestResponseManager::RequestResponseManager(
    std::vector<int>& fdSockets, 
    std::vector<Server*>& servers, 
    SessionManager& sessionManager
) : 
    m_fdSockets(fdSockets), 
    m_servers(servers), 
    m_sessionManager(sessionManager),
    m_epoll(20) 
{}

int RequestResponseManager::findFdLimit() 
{
    std::vector<int> fds;
    int fd;

    while (true) {
        fd = open("/dev/null", O_RDONLY);
        if (fd == -1) 
        {
            if (errno == EMFILE) 
            {
                break;
            } 
            else 
            {
                perror("Erreur lors de l'ouverture de /dev/null");
                return -1;
            }
        }
        fds.push_back(fd);
    }
    for (size_t i = 0; i < fds.size(); ++i) 
    {
        close(fds[i]);
    }
    return fds.size();
}

void RequestResponseManager::run() 
{
    int eventCount;
    struct epoll_event event;
    m_acceptPaused = false;
    fdLimit = findFdLimit();
    if (fdLimit == -1) 
    {
        throw std::runtime_error("Error finding fd limit");
        return;
    }

    try 
    {
        initializeEpoll();
        
        while (!signalReceived) 
        {
            eventCount = m_epoll.wait(-1);
            if (eventCount == -1) 
            {
                Log::error("Erreur lors de epoll_wait");
                break;
            }

            for (int i = 0; i < eventCount; ++i) 
            {
                event = m_epoll.getEvent(i);

                if (event.data.fd == signalPipeFd[0]) 
                {
                    signalReceived = true;
                    break;
                }

                bool isServerSocket = std::find(
                    m_fdSockets.begin(), 
                    m_fdSockets.end(), 
                    event.data.fd
                ) != m_fdSockets.end();//should be ok...

                if (isServerSocket && !m_acceptPaused)
                {
                    if (handleNewConnection(event) == false)
                    {
                        Log::print_final_log("Desactivate new connections", "FD:", event.data.fd);
                        for (std::vector<int>::iterator it = m_fdSockets.begin(); it != m_fdSockets.end(); ++it) 
                        {
                            m_epoll.disableFd(*it);
                        }
                        m_acceptPaused = true;
                    }

                } 
                else if (event.events & EPOLLIN) 
                    processRequest(event.data.fd);

                int activeFdCount = m_epoll.getTotalActiveClients();
                
                if (activeFdCount >= fdLimit)
                {
                    std::ostringstream oss;
                    oss << RED;
                    oss << "Fd Limit reached: " << activeFdCount << "/" << fdLimit;
                    oss << RESET;
                    Log::print_final_log(oss.str(), "FD:", event.data.fd);
                    Log::print_final_log("Erasing all connections", "in", "1 seconds...");
                    m_epoll.check_timeouts_of_clients(m_fdSockets, signalPipeFd[0], m_servers);
                }
                if (activeFdCount < fdLimit && m_acceptPaused == true) 
                {
                    Log::print_final_log("Activate new connections", "FD:", event.data.fd);
                    for (std::vector<int>::iterator it = m_fdSockets.begin(); it != m_fdSockets.end(); ++it) 
                    {
                        m_epoll.enableFd(*it);
                    }
                    m_acceptPaused = false;
                }
            }
        }

        if (signalReceived) 
        {
            m_sessionManager.saveSessionsToFile();
            Log::cleanup();
            throw SignalException();
        }
    }
    catch (std::exception& e) 
    {
        Log::print_final_log("Closing:", e.what());
        Log::print_final_log("Server powered off", "Goodbye!");
    }
}