#include "../headers/signals.hpp"

void handle_sig(int sig)
{
    sig_g = sig;
}