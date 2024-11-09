#include "../headers/cleanup.hpp"
#include "../headers/Log.hpp"


void clearMemory(std::vector<Conf *> Configs)
{
    Log::cleanup();
    clearArray(Configs);
}