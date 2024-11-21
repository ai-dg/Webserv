/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:25 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 20:21:29 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/SignalHandler.hpp"

int signalPipeFd[2];
volatile sig_atomic_t signalReceived = 0;

void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        signalReceived = 1;
        write(signalPipeFd[1], "1", 1);
    }
}

void setupSignalHandler()
{
    if (pipe(signalPipeFd) == -1)
    {
        perror("pipe");
        exit(1);
    }

    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
}

const char* SignalException::what() const throw()
{
    return "Signal received";
}
