#ifndef LOG_HPP
#define LOG_HPP

#include "includes.hpp"
#include <fstream>
#include <ostream>
#include <map>

class Log
{
    private : 
        static std::map<std::string, std::string> files;
        static std::string err_file;
        static std::string access_file;
        Log();
        Log(const Log &cl);
        void operator=(const Log &cl);
        ~Log();
        static void log(std::string path, std::string message);
    public :
        static void init();
        static void init(std::string err, std::string access);
        static void access(std::string message);
        static void error(std::string error);
        static void purgeLog(std::string file);
};



#endif