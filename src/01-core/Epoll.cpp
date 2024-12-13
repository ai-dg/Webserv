/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:57:55 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/13 01:23:31 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/Epoll.hpp"
#include "../00-headers/01-core/Server.hpp"
#include "../00-headers/02-utils/Log.hpp"

std::map<int, std::time_t> Epoll::timers;

/**
 * @brief Coplien Form
 */
Epoll::Epoll(int maxEvents) : maxEvents(maxEvents), totalActiveClients(0)
{
	epoll_fd = epoll_create(maxEvents);
	if (epoll_fd == -1)
	{
		Log::error("epoll_create");
		exit(EXIT_FAILURE);
	}
	events = new epoll_event[maxEvents];
}

Epoll::Epoll(const Epoll &src) : epoll_fd(src.epoll_fd),
	maxEvents(src.maxEvents), events(src.events)
{
	timers = src.timers;
}

Epoll &Epoll::operator=(const Epoll &src)
{
	if (this == &src)
		return (*this);
	epoll_fd = src.epoll_fd;
	maxEvents = src.maxEvents;
	events = src.events;
	timers = src.timers;
	return (*this);
}

Epoll::~Epoll()
{
    if (epoll_fd != -1)
    {
        
        std::map<int, std::time_t>::iterator it;
        for (it = timers.begin(); it != timers.end(); ++it)
        {
            int fd = it->first;
            if (fd != -1)
            {
                Log::print_final_log("Closing connections and fd", "ID:", fd);
                ::close(fd); 
            }
        }
        timers.clear(); 

        
        ::close(epoll_fd);
        epoll_fd = -1;
    }

    
    delete[] events;

    
    Log::cleanup();
}


/**
 * @brief Getters
 */
int Epoll::getFd(void)
{
	return (epoll_fd);
}

/**
 * @brief Epoll functions
 */
int Epoll::wait(int timeout)
{
    int eventCount;

    eventCount = epoll_wait(epoll_fd, events, maxEvents, timeout);
    if (eventCount == -1)
    {
        if (errno == EINTR)
            return (0);
        else
        {
            std::ostringstream errorMsg;
            errorMsg << "epoll_wait failed with error: " << strerror(errno);
            Log::error(errorMsg.str());
            return (-1);
        }
    }
    return (eventCount);
}

bool Epoll::addFd(int fd, uint32_t eventsMask)
{
	struct epoll_event	event;

	if (fd < 0)
	{
		Log::error("Invalid file descriptor passed to addFd");
		return (false);
	}
	std::time_t now = std::time(0);
	Epoll::timers.insert(std::make_pair(fd, now));
	event.data.fd = fd;
	event.events = eventsMask | EPOLLIN;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
	{
		Log::error("Failed to add file descriptor to epoll");
		return (false);
	}
	totalActiveClients++;
	return (true);
}

bool Epoll::removeFd(int& fd)
{
	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1)
	{
		Log::print_final_log("epoll_ctl: removeFd", "FD:", fd);
		return (false);
	}
	if (fd != -1)
	{
		Epoll::timers.erase(fd);
		::close(fd);
		fd = -1;
	}
	totalActiveClients--;
	return (true);
}

struct epoll_event Epoll::getEvent(int index) const
{
	if (index >= 0 && index < maxEvents)
		return (events[index]);
	Log::error("Invalid index in getEvent");
	return (epoll_event());
}

bool Epoll::purgeTimeOutFds(const Conf &conf, int fd, std::vector<Server*>& m_servers)
{
	int	MAX_TIME;

	std::time_t now = std::time(0);
	MAX_TIME = ::atoi(conf.getConfig("keepalive_timeout").c_str());
	std::map<int, std::time_t>::iterator it;
	for (it = Epoll::timers.begin(); it != Epoll::timers.end();)
	{
		if (it->first == fd)
		{
			if (now - it->second > MAX_TIME)
			{
				if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, it->first, NULL) == -1)
				{
					Log::print_final_log("Error erasing fd with epollctl in server", strerror(errno));
					return (false);
				}
				for (size_t i = 0; i < m_servers.size(); i++)
					m_servers[i]->removeFdClient(it->first);
				if (it->first != -1)
					::close(it->first);
				Epoll::timers.erase(it++);
				totalActiveClients--; 
				Log::print_final_log("keepalive_timeout finished:", MAX_TIME, "ID:", epoll_fd);
				return (true);
			}
		}
		it++;
		
	}
	return (false);
}

void Epoll::check_timeouts_of_clients(const std::vector<int>& m_fdSockets, int signalPipefd, std::vector<Server*>& m_servers) 
{
    std::time_t now = std::time(0); 
    int timeout_seconds = 1;

    std::map<int, std::time_t>::iterator it = timers.begin();
    while (it != timers.end()) 
    {
        int fd = it->first;        
        if (std::find(m_fdSockets.begin(), m_fdSockets.end(), fd) != m_fdSockets.end()) 
        {
            ++it;
            continue;
        }
		if (fd == signalPipefd) 
		{
			++it;
			continue;
		}    
        if (now - it->second > timeout_seconds) 
        {    
            if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) 
            {
                Log::print_final_log("Error erasing fd with epollctl in global", strerror(errno));
            }
            if (close(fd) == -1) 
            {
                Log::print_final_log("Error closing the fd", strerror(errno));
            }
			for (size_t i = 0; i < m_servers.size(); i++)
				m_servers[i]->removeFdClient(fd);
			totalActiveClients--;
            timers.erase(it++); 
        } 
        else 
        {
            ++it; 
        }
    }
}


int Epoll::makeSocketNonBlocking(int fd)
{
	int	flags;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		throw std::runtime_error("fcntl GETFL failed");
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) == -1)
		throw std::runtime_error("fcntl SETFL failed");
	return 0;
}

void Epoll::disableFd(int fd) 
{
    struct epoll_event ev = {};
    ev.data.fd = fd;
    ev.events = 0;
    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev);
}

void Epoll::enableFd(int fd) 
{
    struct epoll_event ev = {};
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev);
}

int Epoll::getTotalActiveClients() const
{
	return totalActiveClients;
}