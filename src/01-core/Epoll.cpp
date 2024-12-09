/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:57:55 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/09 10:31:33 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/Epoll.hpp"
#include "../00-headers/02-utils/Log.hpp"

std::map<int, std::time_t> Epoll::timers;

/**
 * @brief Coplien Form
 */
Epoll::Epoll(int maxEvents) : maxEvents(maxEvents)
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
	return (true);
}

struct epoll_event Epoll::getEvent(int index) const
{
	if (index >= 0 && index < maxEvents)
		return (events[index]);
	Log::error("Invalid index in getEvent");
	return (epoll_event());
}

bool Epoll::purgeTimeOutFds(const Conf &conf, int epoll_fd)
{
	int	MAX_TIME;

	std::time_t now = std::time(0);
	MAX_TIME = atoi(conf.getConfig("keepalive_timeout").c_str());
	std::map<int, std::time_t>::iterator it;
	for (it = Epoll::timers.begin(); it != Epoll::timers.end();)
	{
		if (now - it->second > MAX_TIME)
		{
			if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, it->first, NULL) == -1)
			{
				Log::error("epoll_ctl: removeFd");
				return (false);
			}
			if (it->first != -1)
				::close(it->first);
			Epoll::timers.erase(it++);
		}
		else
			it++;
	}
	return (true);
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
