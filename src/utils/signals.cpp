#include "../headers/signals.hpp"

void handle_sig(int sig)
{
    sig_g = sig;
    std::cerr << std::endl << sig_g << " signal captured" << std::endl;

}