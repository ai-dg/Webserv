/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:57:09 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 20:40:12 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../00-shared/includes.hpp"

class Log
{
    class LogStream
    {
        private:
            std::string filePath;
            std::ofstream fileStream;
            std::ostringstream buffer;
            static const size_t MAX_LINES = 2000;

            /**
             * @brief Truncate the file if it exceeds the maximum number of lines
             */
            void truncateFileIfNeeded(const std::string& newMessage);
    
        public:
            /**
             * @brief Coplien form
             */
            LogStream(std::string const& path);
            LogStream(LogStream const& src);
            LogStream& operator=(LogStream const& src);
            ~LogStream();

            /**
             * @brief Overload of the << operator
             */
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
        
       
    };

    private :
        static std::map<std::string, LogStream*> logStreams;
        static std::map<std::string, std::string> files;
        static std::string err_file;
        static std::string access_file;
        static std::string debug_file;
        
        /**
         * @brief Coplien form
         */
        Log();
        Log(Log const& src);
        void operator=(Log const& src);
        ~Log();
        
        /**
         * @brief Initialize the log files
         */
        static void log(std::string path, std::string message);

    public :

        /**
         * @brief Public functions
         */
        static void cleanup();
        static void init();
        static void init(std::string err, std::string access);
        static void access(std::string message);
        static void error(std::string error);
        static void debug(std::string debug);
        static void purgeLog(std::string file);
        static LogStream& output(const std::string& path); 
};
