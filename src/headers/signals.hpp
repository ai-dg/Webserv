#ifndef SIGNALS_HPP
#define SIGNALS_HPP

#include <csignal>
#include <iostream>

extern volatile sig_atomic_t sig_g;

void handle_sig(int sig);


#endif

