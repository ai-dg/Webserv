/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:57:55 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/04 21:33:47 by dagudelo         ###   ########.fr       */
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
	// Log::output("./sessions/epoll.log") << "Epoll class object created" << std::endl;
}

Epoll::Epoll(const Epoll &src) : epoll_fd(src.epoll_fd),
	maxEvents(src.maxEvents), events(src.events)
{
	timers = src.timers;
	// Log::output("./sessions/epoll.log") << "Epoll class object copied" << std::endl;
}

Epoll &Epoll::operator=(const Epoll &src)
{
	if (this == &src)
		return (*this);
	epoll_fd = src.epoll_fd;
	maxEvents = src.maxEvents;
	events = src.events;
	timers = src.timers;
	// Log::output("./sessions/epoll.log") << "Epoll class object assigned" << std::endl;
	return (*this);
}

Epoll::~Epoll()
{
	close(epoll_fd);
	timers.clear();
	delete[] events;
	// Log::output("./sessions/epoll.log") << "Epoll class object destroyed" << std::endl;
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
// int Epoll::wait(int timeout)
// {
// 	int	eventCount;

// 	Log::debug("Starting epoll_wait...");
// 	eventCount = epoll_wait(epoll_fd, events, maxEvents, timeout);
// 	if (eventCount == -1)
// 	{
// 		if (errno == EINTR)
// 		{
// 			Log::debug("epoll_wait interrupted by a signal");
// 			return (0);
// 		}
// 		else
// 		{
// 			std::ostringstream errorMsg;
// 			errorMsg << "epoll_wait failed with error: " << strerror(errno);
// 			Log::error(errorMsg.str());
// 			return (-1);
// 		}
// 	}
// 	std::ostringstream successMsg;
// 	successMsg << "epoll_wait returned with " << eventCount << " events";
// 	Log::debug(successMsg.str());
// 	return (eventCount);
// }


int Epoll::wait(int timeout)
{
    int eventCount;

    Log::debug("Starting epoll_wait...");
    eventCount = epoll_wait(epoll_fd, events, maxEvents, timeout);
    if (eventCount == -1)
    {
        if (errno == EINTR)
        {
            Log::debug("epoll_wait interrupted by a signal");
            return (0);
        }
        else
        {
            std::ostringstream errorMsg;
            errorMsg << "epoll_wait failed with error: " << strerror(errno);
            Log::error(errorMsg.str());
            return (-1);
        }
    }

    // Log du nombre d'événements détectés
    std::ostringstream successMsg;
    successMsg << "epoll_wait returned with " << eventCount << " events";
    Log::debug(successMsg.str());

    // Ajout des détails pour chaque événement
    for (int i = 0; i < eventCount; ++i)
    {
        std::ostringstream eventMsg;
        eventMsg << "Event " << i << ": fd=" << events[i].data.fd 
                 << ", events=" << events[i].events;

        // Détailler les types d'événements
        if (events[i].events & EPOLLIN) {
            eventMsg << " [EPOLLIN]";
        }
        if (events[i].events & EPOLLOUT) {
            eventMsg << " [EPOLLOUT]";
        }
        if (events[i].events & EPOLLHUP) {
            eventMsg << " [EPOLLHUP]";
        }
        if (events[i].events & EPOLLERR) {
            eventMsg << " [EPOLLERR]";
        }
        if (events[i].events & EPOLLRDHUP) {
            eventMsg << " [EPOLLRDHUP]";
        }

        Log::debug(eventMsg.str());
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
	
	std::ostringstream logMsg;
	logMsg << "File descriptor " << fd << " added to epoll with events: " << eventsMask;
	Log::debug(logMsg.str());
	
	

	Log::debug("File descriptor added to epoll successfully");
	return (true);
}

bool Epoll::removeFd(int fd)
{
	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1)
	{
		Log::error("Failed to remove file descriptor from epoll");
		return (false);
	}
	if (close(fd) == -1)
		Log::error("Failed to close file descriptor");
	else
		Log::debug("File descriptor closed successfully");
	Epoll::timers.erase(fd);
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
			close(it->first);
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
	{
		Log::error("fcntl");
		return (-1);
	}
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) == -1)
	{
		Log::error("fcntl");
		return -1;
	}
	return 0;
}
