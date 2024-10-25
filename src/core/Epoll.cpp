#include "../headers/Epoll.hpp"
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

Epoll::Epoll(int maxEvents) : maxEvents(maxEvents)
{
    
    epollFd = epoll_create(maxEvents);
    if (epollFd == -1) 
    {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }
    
    events = new epoll_event[maxEvents];
}

Epoll::~Epoll() 
{
    close(epollFd); 
    delete[] events;
}

bool Epoll::addFd(int fd, uint32_t eventsMask) 
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = eventsMask;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event) == -1) 
    {
        perror("epoll_ctl: addFd");
        return false;
    }
    return true;
}

bool Epoll::removeFd(int fd) 
{
    if (epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL) == -1) 
    {
        perror("epoll_ctl: removeFd");
        return false;
    }
    return true;
}

int Epoll::wait(int timeout) 
{
    int eventCount = epoll_wait(epollFd, events, maxEvents, timeout);
    if (eventCount == -1) 
    {
        perror("epoll_wait");
        exit(EXIT_FAILURE);
    }
    return eventCount;
}

struct epoll_event Epoll::getEvent(int index) const 
{
    if (index >= 0 && index < maxEvents)
    {
        return events[index];
    }
    return epoll_event();  
}

int Epoll::makeSocketNonBlocking(int fd) 
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) 
    {
        perror("fcntl");
        return -1;
    }
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) 
    {
        perror("fcntl");
        return -1;
    }
    return 0;
}
