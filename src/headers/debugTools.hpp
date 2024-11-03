#ifndef DEBUGTOOLS_HPP
#define DEBUGTOOLS_HPP

#include <iostream>
#include "colors.hpp"
#include "Log.hpp"

template<typename T>
void printContenerValues(T & array)
{
    typename T::iterator it;
    for (it = array.begin(); it != array.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
}

template<typename T>
void printContenerValues(T & array, std::string Color)
{
    typename T::iterator it;
    for (it = array.begin(); it != array.end(); ++it)
    {
        Log::output("./sessions/Server.txt") << Color << *it << RESET << std::endl;
    }
}


#endif
