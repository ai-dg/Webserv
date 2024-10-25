#include "../headers/Log.hpp"


std::string Log::err_file = "/logs/error.log";

std::string Log::access_file = "/logs/access.log";
 
Log::Log()
{
}
Log::Log(const Log &cl)
{
    (void) cl;
}
void Log::operator=(const Log &cl)
{
    (void) cl;
}
Log::~Log()
{
}

 void Log::init(std::string err, std::string access)
 {
    Log::err_file = err;
    Log::access_file = access;
 }

 void Log::log(std::string path, std::string message)
{
    std::ofstream file;
    file.open(path.c_str(), std::ios::app);
    file << message << std::endl;
    file.close();
}

 void Log::access(std::string message)
 {
    Log::log(access_file, message);
 }

 void Log::error(std::string error)
 {
    Log::log(err_file, error);
 }