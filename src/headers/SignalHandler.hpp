#ifndef SIGNALHANDLER_HPP
#define SIGNALHANDLER_HPP

#include <csignal>
#include <stdexcept>
#include <unistd.h>

extern int signalPipeFd[2];
extern volatile sig_atomic_t signalReceived;

void signalHandler(int signal);
void setupSignalHandler();

class SignalException : public std::exception
{
public:
    virtual const char* what() const throw()
    {
        return "Signal received";
    }
};

#endif
