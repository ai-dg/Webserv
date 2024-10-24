#ifndef EPOLL_HPP
#define EPOLL_HPP

#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>

class Epoll 
{
    private:
        int epollFd;                    
        int maxEvents;                  
        struct epoll_event *events;
     
    public:
        Epoll(int maxEvents);
        ~Epoll();
       
        bool addFd(int fd, uint32_t events);
        bool removeFd(int fd);
        int wait(int timeout);
        struct epoll_event getEvent(int index) const;
        int makeSocketNonBlocking(int fd);
};


#endif
