#include "../headers/Epoll.hpp"
#include "../headers/Log.hpp"
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <cstring>

std::map<int, std::time_t> Epoll::timers; 

Epoll::Epoll(int maxEvents) : maxEvents(maxEvents)
{
    
    epollFd = epoll_create(maxEvents);
    if (epollFd == -1) 
    {
        Log::error("epoll_create");
        exit(EXIT_FAILURE);
    }
    
    events = new epoll_event[maxEvents];
}

int Epoll::getFd(void)
{
    return epollFd;
}

Epoll::~Epoll() 
{
    close(epollFd);
    timers.clear();
    delete[] events;
}

// bool Epoll::addFd(int fd, uint32_t eventsMask) 
// {
//     if (fd < 0)
//     {
//         Log::error("fail opening file socket");
//         return false;
//     }
//     std::time_t now = std::time(0);
//     Epoll::timers.insert(std::pair<int, std::time_t>(fd, now));
//     struct epoll_event event;
//     event.data.fd = fd;
//     event.events = eventsMask;
//     if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event) == -1) 
//     {
//         Log::error("epoll_ctl: addFd");
//         return false;
//     }
//     return true;
// }

bool Epoll::addFd(int fd, uint32_t eventsMask) 
{
    if (fd < 0)
    {
        Log::error("Invalid file descriptor passed to addFd");
        return false;
    }

    std::time_t now = std::time(0);
    Epoll::timers.insert(std::make_pair(fd, now));

    struct epoll_event event;
    event.data.fd = fd;
    event.events = eventsMask;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event) == -1) 
    {
        Log::error("Failed to add file descriptor to epoll");
        return false;
    }

    Log::debug("File descriptor added to epoll successfully");
    return true;
}


bool Epoll::purgeTimeOutFds(const Conf &conf, int epollFd)
 {
    std::time_t now = std::time(0);
    int MAX_TIME = atoi(conf.getConfig("keepalive_timeout").c_str());
    std::map<int, std::time_t>::iterator it;
    for (it = Epoll::timers.begin(); it != Epoll::timers.end();)
    {
        if (now - it->second > MAX_TIME)
        {
            if (epoll_ctl(epollFd, EPOLL_CTL_DEL, it->first, NULL) == -1) 
            {
                Log::error("epoll_ctl: removeFd");
                return false;
            }
            close(it->first);
            Epoll::timers.erase(it++);
        }
        else
            it++;
    }
    return true;
 }

// bool Epoll::removeFd(int fd)
// {
//     if (epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL) == -1) 
//     {
//         Log::error("epoll_ctl: removeFd");
//         return false;
//     }
//     Epoll::timers.erase(fd);
//     return true;
// }

bool Epoll::removeFd(int fd)
{
    if (epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL) == -1) 
    {
        Log::error("Failed to remove file descriptor from epoll");
        return false;
    }

    if (close(fd) == -1)
    {
        Log::error("Failed to close file descriptor");
    }
    else
    {
        Log::debug("File descriptor closed successfully");
    }

    Epoll::timers.erase(fd);
    return true;
}


// int Epoll::wait(int timeout) 
// {
//     int eventCount = epoll_wait(epollFd, events, maxEvents, timeout);
//     // if (eventCount == -1) 
//     // {
//     //     Log::error("epoll_wait");
//     //     exit(EXIT_FAILURE);
//     // }
//     if (eventCount == -1) 
//     {
//         if (errno == EINTR)
//         {
//             // Interruption par un signal, retournez 0 pour indiquer aucun événement
//             return 0;
//         }
//         else
//         {
//             Log::error("epoll_wait failed");
//             return -1; // Retournez -1 pour indiquer une erreur fatale
//         }
//     }
//     return eventCount;
// }

int Epoll::wait(int timeout) 
{
    Log::debug("Starting epoll_wait...");
    int eventCount = epoll_wait(epollFd, events, maxEvents, timeout);

    if (eventCount == -1) 
    {
        if (errno == EINTR)
        {
            Log::debug("epoll_wait interrupted by a signal");
            return 0; // Aucun événement, mais pas une erreur fatale
        }
        else
        {
            std::ostringstream errorMsg;
            errorMsg << "epoll_wait failed with error: " << strerror(errno);
            Log::error(errorMsg.str());
            return -1; // Erreur fatale
        }
    }

    std::ostringstream successMsg;
    successMsg << "epoll_wait returned with " << eventCount << " events";
    Log::debug(successMsg.str());
    return eventCount;
}

// struct epoll_event Epoll::getEvent(int index) const 
// {
//     if (index >= 0 && index < maxEvents)
//     {
//         return events[index];
//     }
//     return epoll_event();  
// }

struct epoll_event Epoll::getEvent(int index) const 
{
    if (index >= 0 && index < maxEvents)
    {
        return events[index];
    }
    Log::error("Invalid index in getEvent");
    return epoll_event();  
}


int Epoll::makeSocketNonBlocking(int fd) 
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) 
    {
        Log::error("fcntl");
        return -1;
    }
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) 
    {
        Log::error("fcntl");
        return -1;
    }
    return 0;
}
