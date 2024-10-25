#ifndef EPOLL_HPP
#define EPOLL_HPP

#include <sys/epoll.h>
#include <unistd.h>
#include <map>
#include <ctime>
#include <iostream>
#include "Conf.hpp"

class Epoll 
{
    private:
        int epollFd;                 
        int maxEvents;
        struct epoll_event *events;
     
    public:
        static std::map<int, std::time_t> timers;
        Epoll(int maxEvents);
        ~Epoll();
        int getFd();
        static bool purgeTimeOutFds(const Conf &conf, int epollFd);
        bool addFd(int fd, uint32_t events);
        bool removeFd(int fd);
        int wait(int timeout);
        struct epoll_event getEvent(int index) const;
        int makeSocketNonBlocking(int fd);
};


#endif
