#include "../headers/SignalHandler.hpp"
#include <unistd.h>
#include <cstdlib>
#include <csignal>
#include <iostream>
#include <cstdio>


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
