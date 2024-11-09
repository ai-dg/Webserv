#ifndef CLEANUP_HPP
#define CLEANUP_HPP

#include "../headers/Server.hpp"
#include "../headers/Conf.hpp"

void clearMemory(std::vector<Conf *> Configs);

template<typename T>
void clearArray(std::vector<T*> &array)
{
    typename std::vector<T*>::iterator it;
    for (it = array.begin(); it != array.end(); ++it)
    {
        delete *it;
    }
    array.clear();
}

template<typename T, typename U>
void clearMaps(std::map<T, U*> &map)
{
    typename std::map<T, U*>::iterator it;
    for (it = map.begin(); it != map.end(); ++it)
    {
        delete it->second;
    }
    map.clear();
}

#endif
