#ifndef LOG_HPP
#define LOG_HPP

#include "includes.hpp"
#include <fstream>
#include <ostream>
#include <map>
#include <string>
#include <deque>
#include <sstream>

class Log
{
    class LogStream
    {
        public:
            LogStream(std::string const& path);
            ~LogStream();

            LogStream& operator<<(std::ostream& (*manip)(std::ostream&));
            
            
            template<typename T>
            LogStream& operator<<(T const& data) 
            {
                if (!fileStream.is_open()) 
                {
                    return *this;
                }
                buffer << data;
                truncateFileIfNeeded(buffer.str());
                return *this;
            }
        
        private:
            std::string filePath;
            std::ofstream fileStream;
            std::ostringstream buffer;
            static const size_t MAX_LINES = 2000;

            void truncateFileIfNeeded(const std::string& newMessage);


    };

    private :
        static std::map<std::string, LogStream*> logStreams;
        static std::map<std::string, std::string> files;
        static std::string err_file;
        static std::string access_file;
        static std::string debug_file;
        Log();
        Log(const Log &cl);
        void operator=(const Log &cl);
        ~Log();
        static void log(std::string path, std::string message);
        void cleanup();

    public :
        static void init();
        static void init(std::string err, std::string access);
        static void access(std::string message);
        static void error(std::string error);
        static void debug(std::string debug);
        static void purgeLog(std::string file);
        static LogStream& output(const std::string& path); 

};

#endif