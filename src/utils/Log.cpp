#include "../headers/Log.hpp"
#include <deque>
#include <string>
#include <fstream>
#include <sstream>
#include <istream>
#include <iostream>

std::map<std::string, std::string> Log::files;
std::string Log::err_file = "logs/error.log";
std::string Log::debug_file = "logs/debug.log";
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
    cleanup();
}

void Log::purgeLog(std::string filename)
{
    if (filename != "error" && filename != "access"  && filename != "debug")
        return;
    std::ofstream file;
    file.open(Log::files[filename].c_str());
    file << "";
}

void Log::init()
{
    Log::files.insert(std::make_pair("debug", Log::debug_file));
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

//  void Log::log(std::string path, std::string message)
// {
//     std::ofstream file;
//     file.open(path.c_str(), std::ios::app);
//     file << message << std::endl;
//     file.close();
// }


void Log::log(std::string path, std::string message)
{
    const size_t MAX_LINES = 2000; 

    
    std::ifstream fileRead(path.c_str());
    std::deque<std::string> lines;
    std::string line;

    
    while (std::getline(fileRead, line)) 
    {
        lines.push_back(line);
        if (lines.size() >= MAX_LINES) 
        {
            lines.pop_back(); 
        }
    }
    fileRead.close();

    
    lines.push_front(message);

    
    std::ofstream fileWrite(path.c_str(), std::ios::trunc);
    for (std::deque<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it) 
    {
        fileWrite << *it << std::endl;
    }
    fileWrite.close();
}

void Log::access(std::string message)
{
    Log::log(Log::access_file, message);
}

void Log::debug(std::string message)
{
    Log::log(Log::debug_file, message);
}

void Log::error(std::string error)
{
    Log::log(Log::err_file, error);
}

Log::LogStream::LogStream(const std::string &path) : filePath(path), fileStream(path.c_str(), std::ofstream::app) 
{
    if (!fileStream.is_open()) 
    {
        Log::error("Erreur : Impossible d'ouvrir le fichier de log");
    }

}

Log::LogStream::~LogStream() 
{

}

Log::LogStream& Log::LogStream::operator<<(std::ostream& (*manip)(std::ostream&)) 
{
    if (!fileStream.is_open()) 
    {
        return *this;
    }

    
    if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) 
    {
        truncateFileIfNeeded(buffer.str());  
        buffer.str("");  
        buffer.clear();
    } 
    else 
    {
        buffer << manip;  
    }
    return *this;
}

void Log::LogStream::truncateFileIfNeeded(const std::string& newMessage)
{
    const size_t MAX_LINES = 2000;
    std::deque<std::string> lines;
    std::string line;

    
    std::ifstream fileRead(filePath.c_str());
    if (!fileRead.is_open()) 
    {
        Log::error("Erreur : Impossible d'ouvrir le fichier en lecture dans la classe Log");
        return;
    }
    while (std::getline(fileRead, line))
    {
        lines.push_back(line);
    }
    fileRead.close();

    
    lines.push_front(newMessage);

    
    while (lines.size() > MAX_LINES)
    {
        lines.pop_back();
    }

    
    std::ofstream fileWrite(filePath.c_str(), std::ios::trunc);
    if (!fileWrite.is_open()) 
    {
        Log::error("Erreur : Impossible d'ouvrir le fichier en écriture dans la classe Log");
        return;
    }
    for (std::deque<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it)
    {
        fileWrite << *it;
        if (it + 1 != lines.end()) 
        {
            fileWrite << "\n";
        }
    }
    fileWrite.close();
}

std::map<std::string, Log::LogStream*> Log::logStreams;

Log::LogStream& Log::output(const std::string& path)
{
    
    if (logStreams.find(path) == logStreams.end()) {
        
        logStreams[path] = new LogStream(path);
    }
    
    return *logStreams[path];
}

void Log::cleanup()
{
    for (std::map<std::string, LogStream*>::iterator it = logStreams.begin(); it != logStreams.end(); ++it) {
        delete it->second;
    }
    logStreams.clear();
}