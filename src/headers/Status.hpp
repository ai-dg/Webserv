#ifndef STATUS_HPP
#define STATUS_HPP

#include <map>
#include <iostream>

class Status
{
    private:
        static std::map<int, std::string> initializeCodes();
        static const std::map<int, std::string>codes;
    public:
        static std::string get(int code);
};

#endif