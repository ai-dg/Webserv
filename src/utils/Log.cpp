#include "../headers/Log.hpp"

std::map<std::string, std::string> Log::files;
std::string Log::err_file = "logs/error.log";
std::string Log::access_file = "logs/access.log";

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

void Log::purgeLog(std::string filename)
{
    if (filename != "error" && filename != "access")
        return;
    std::ofstream file;
    file.open(Log::files[filename].c_str());
    file << "";
}

void Log::init()
{
    Log::files.insert(std::make_pair("error", Log::err_file));
    Log::files.insert(std::make_pair("access", Log::access_file));
}

void Log::init(std::string err, std::string access)
{
    Log::err_file = err;
    Log::files["error"] = err;
    Log::access_file = access;
    Log::files["access"] = access;
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
    Log::log(Log::access_file, message);
 }

 void Log::error(std::string error)
 {
    Log::log(Log::err_file, error);
 }