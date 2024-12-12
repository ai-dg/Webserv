/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:55:43 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/12 22:21:18 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../00-shared/includes.hpp"
#include "../01-core/Conf.hpp"

class Epoll 
{
    private:
        int epoll_fd;                 
        int maxEvents;
        struct epoll_event *events;
        static std::map<int, std::time_t> timers;
     
    public:
        /**
         * @brief Coplien Form
         */
        Epoll(int maxEvents);
        Epoll(const Epoll &src);
        Epoll &operator=(const Epoll &src);
        ~Epoll();

        /**
         * @brief Getters
         */
        int getFd();

        /**
         * @brief Epoll functions
         */
        int wait(int timeout);
        bool addFd(int fd, uint32_t events);
        bool removeFd(int& fd);
        struct epoll_event getEvent(int index) const;
        static bool purgeTimeOutFds(const Conf &conf, int epoll_fd);
        int makeSocketNonBlocking(int fd);
        void check_timeouts_of_clients();
};
