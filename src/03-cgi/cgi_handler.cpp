/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:59:06 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/05 19:15:47 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/HttpRequest.hpp"
#include "../00-headers/01-core/scriptUtils.hpp"
#include "../00-headers/01-core/Pipe.hpp"
#include "../00-headers/02-utils/Log.hpp"
#include "../00-headers/02-utils/files.hpp"
#include "../00-headers/02-utils/date.hpp"
#include "../00-headers/03-cgi/cgi_handler.hpp"


/**
 * @brief Prepare the environment for the CGI script
 */
std::string Cgi_handler::getExeContext(std::string file)
{
    if (file.find(".") == std::string::npos)
        return "bash";
    if (file.find(".php") != std::string::npos)
        return "php-cgi";
    if (file.find(".py") != std::string::npos)
        return "python";
    if (file.find(".pl") != std::string::npos)
        return "perl";
    if (file.find(".sh") != std::string::npos)
        return "bash";
    return getContextFromFile(file);;
}

void Cgi_handler::addToEnvironment(const char * env)
{
    if (env)
    {
        this->environment.push_back(strdup(const_cast<char *>(env)));
    }
}

void Cgi_handler::addToEnvironment(std::string env)
{  
    this->environment.push_back(strdup(const_cast<char*>(env.c_str())));
}

std::string createBufferDataFile(std::string body)
{
    (void)body;
    std::string filename = "./sessions/tmp.d";
    return filename;
}

void Cgi_handler::setEnvironment(HttpRequest &req)
{   
    std::map<std::string, std::string> headers = req.getHeaders();
    std::string requestMethodEnv = "REQUEST_METHOD=" + req.getMethod();
    std::string contentLengthEnv;

    if (req.getMethod() == "POST" || req.getMethod() == "DELETE")
    {
        if (req.getHeader("Content-Length").size() != 0)
        {
            contentLengthEnv =  "CONTENT_LENGTH=" + req.getHeader("Content-Length");
        }
        else if (req.getHeader("Content-Length").size() == 0 && req.getHeader("Transfer-Encoding") == "chunked")
        {
            std::ostringstream oss;
            oss << req.getBody().length();
            contentLengthEnv = "CONTENT_LENGTH=" + oss.str();
        }
    }
    else
        contentLengthEnv = ""; 
    this->addToEnvironment(requestMethodEnv);
    this->addToEnvironment(contentLengthEnv[0] ? const_cast<char*>(contentLengthEnv.c_str()) : NULL);
    
    size_t queryPos = scriptPath.find('?');
    if (queryPos != std::string::npos) {
        std::string queryString = scriptPath.substr(queryPos + 1);
        this->addToEnvironment("QUERY_STRING=" + queryString);
    }
    std::map<std::string, std::string>::iterator it;
    for (it = headers.begin(); it != headers.end(); ++it)
    {
        if (it->second == "chunked")
            continue;
        if (it->first.find("Cookie") != std::string::npos)
        {
            std::string cookieHeader = it->second;
            size_t start = 0;
            size_t end = 0;
            while ((end = cookieHeader.find(';', start)) != std::string::npos)
            {
                std::string cookie = cookieHeader.substr(start, end - start);
                start = end + 2; 
                this->addToEnvironment(cookie);
            }
            if (start < cookieHeader.size())
            {
                std::string cookie = cookieHeader.substr(start);
                this->addToEnvironment(cookie);
            }
        }
        else
            this->addToEnvironment(req.getFormatedHeader(it->first));
    }
    this->addToEnvironment("CONTENT_TYPE=" + req.getHeader("Content-Type"));
    this->addToEnvironment("REDIRECT_STATUS=1");
    this->addToEnvironment("SERVER_PROTOCOL=HTTP/1.1");
    this->addToEnvironment("PATH_INFO=/");
    if (getExeContext(scriptPath) == "php-cgi")
    {
        this->addToEnvironment("SCRIPT_NAME=" + scriptPath);
        this->addToEnvironment("SCRIPT_FILENAME=" + scriptPath);        
    }
    else if (getExeContext(scriptPath) == "python3")
        this->addToEnvironment("PYTHONWARNINGS=ignore");
    else
    {
        std::string filename = createBufferDataFile(req.getBody());
        this->addToEnvironment("CGI_FILE=" + filename);
    }
    environment.push_back(NULL);
}

void Cgi_handler::debugEnvironment()
{
    std::cerr << BOLD_VIOLET ;
    std::cerr << "start printing env -------------------------------------------------" << std::endl;
    int i = 0;
    while (environment[i])
    {
        std::cerr << environment[i] << std::endl;
        i++;
    }
    std::cerr << "end printing env ---------------------------------------------------" << std::endl;
    std::cerr << RESET;
}

/**
 * @brief Coplien form
 */

Cgi_handler::Cgi_handler()
{

}

Cgi_handler::Cgi_handler(Cgi_handler const& src)
{
    *this = src;
}

Cgi_handler& Cgi_handler::operator=(Cgi_handler const& src)
{
    if (this != &src)
    {
        this->scriptPath = src.scriptPath;
        this->queryString = src.queryString;
        this->fd_client = src.fd_client;
        this->environment = src.environment;
    }
    return *this;
}

Cgi_handler::~Cgi_handler()
{    
    for (size_t i = 0; i < environment.size(); ++i)
        delete environment[i];
    environment.clear();
}

/**
 * @brief Execute the CGI script
 */


void Cgi_handler::executeCGI(std::string const& scriptPath, HttpRequest &req, int fd_client, HttpResponse &res) 
{
    pid_t pid;
    Pipe pipe_in("./sessions/pipe_infile");
    Pipe pipe_out("./sessions/pipe_outfile");
    this->scriptPath = scriptPath;
    std::string data = req.getBody();

    if (pipe_in.getFd() == -1 || pipe_out.getFd() == -1) {
        std::cerr << "Error: Failed to open temporary files for Pipe." << std::endl;
        return;
    }
    pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }
    if (pid == 0) 
    {
        ::lseek(pipe_in.getFd(), 0, SEEK_SET);
        if (dup2(pipe_in.getFd(), STDIN_FILENO) == -1) 
        {
            ::perror("dup2 stdin");
            ::exit(1);
        }
        if (::dup2(pipe_out.getFd(), STDOUT_FILENO) == -1) 
        {
            ::perror("dup2 stdout");
            ::exit(1);
        }
        pipe_in.closeFd();
        pipe_out.closeFd();
        setEnvironment(req);
        std::string scriptPathTemp = scriptPath;
        size_t queryPos = scriptPathTemp.find('?');
        if (queryPos != std::string::npos)
            scriptPathTemp = scriptPathTemp.substr(0, queryPos);
        std::string exe_context = getExeContext(scriptPathTemp);
        std::string path = "/usr/bin/env";
        if (req.hasFileSpecialRoute(scriptPathTemp))
        {
            Location *route = req.getRouteConf(getExtension(scriptPathTemp));
            if (!route)
                std::cerr << "unkown route" << std::endl;
            if (route && !route->exe().empty())
            {   
                if (resolvePath(route->exe()))        
                    path.assign(resolvePath(route->exe()));
            }
            if (resolvePath(scriptPathTemp))           
                exe_context.assign(resolvePath(scriptPathTemp));
            else
                exe_context.clear();
            scriptPathTemp.clear();
        }
        char *const argv[] = 
        {
            const_cast<char *>(path.c_str()),
            const_cast<char *>(exe_context.c_str()),
            const_cast<char *>(scriptPathTemp.c_str()),
            NULL
        };
        if (execve(argv[0], argv, environment.data()) == -1)
        {
            ::perror("execve");
            ::exit(1);
        }
    } 
    else 
    {  
        size_t offset = 0;
        ssize_t bytes_written;
        while (offset < data.size()) 
        {
            bytes_written = ::write(pipe_in.getFd(), data.c_str() + offset, data.size() - offset);
            if (bytes_written == -1) 
            {

                perror("write");
                pipe_out.closeFd();
                return;
            }
            offset += bytes_written;
        }
        pipe_in.closeFd(); 
        int status;
        pid_t wpid = waitpid(pid, &status, 0);
        if (wpid == -1) {
            perror("waitpid");
            return;
        }

        if (WIFEXITED(status)) 
        {
            if (WEXITSTATUS(status) == 1 || WEXITSTATUS(status) == 126 || WEXITSTATUS(status) == 127 || WEXITSTATUS(status) > 128)
            {
                res.setRedirection(500);
                res.send(fd_client);
                return;
            }
        } 
        else if (WIFSIGNALED(status)) 
        {
            res.setRedirection(500);
            res.send(fd_client);
            return;
        } 
        else 
        {
            res.setRedirection(500);
            res.send(fd_client);
            return;
        }
        char buffer[4096];
        ssize_t bytesRead;
        ::lseek(pipe_out.getFd(), 0, SEEK_SET);
        std::string bufferAccumulator;
        std::string bufferAccumulator2;
        if (scriptPath.find(".php") != std::string::npos) 
            write(fd_client, "HTTP/1.1 200 OK\r\n", 17);
        if (scriptPath.find(".bla") != std::string::npos) 
        {
            std::ostringstream headers;
            headers << "HTTP/1.1 200 OK" << CRLF
                    << "Content-Length: " << data.size() << CRLF
                    << "Content-Type: text/html; charset=utf-8" << CRLF
                    << "Date: " << get_current_date() << CRLF << CRLF;

            std::string headersStr = headers.str();
            size_t offset = 0;
            ssize_t bytesWritten;
            while (offset < headersStr.size()) 
            {
                bytesWritten = ::write(fd_client, headersStr.c_str() + offset, headersStr.size() - offset);
                if (bytesWritten == -1) 
                {
                    ::usleep(1000); 
                    continue;
                } 
                else if (bytesWritten == 0) 
                {
                    ::perror("write returned 0 (unexpected)");
                    return;
                }
                offset += bytesWritten; 
            }
            bool headersSkipped = false;
            bufferAccumulator2.append(headersStr);
            std::string remainingBuffer;
            while ((bytesRead = ::read(pipe_out.getFd(), buffer, sizeof(buffer) - 1)) > 0) {
                buffer[bytesRead] = '\0';
                remainingBuffer += buffer;

                
                if (!headersSkipped) 
                {
                    size_t headerEnd = remainingBuffer.find("\r\n\r\n");
                    if (headerEnd != std::string::npos) 
                    {
                        headersSkipped = true;
                        remainingBuffer = remainingBuffer.substr(headerEnd + 4);
                    } 
                    else 
                        continue;
                }
                offset = 0;
                while (offset < remainingBuffer.size()) 
                {
                    bytesWritten = ::write(fd_client, remainingBuffer.c_str() + offset, remainingBuffer.size() - offset);
                    if (bytesWritten == -1) 
                    {
                        ::usleep(1000); 
                        continue;
                    } 
                    else if (bytesWritten == 0) 
                    {
                        ::perror("write returned 0 (unexpected)");
                        break;
                    }

                    offset += bytesWritten; 
                }
                bufferAccumulator2 += remainingBuffer; 
                remainingBuffer.clear();               
            }
            if (bytesRead == -1) 
                ::perror("read");
        }
        else 
        {
            while ((bytesRead = ::read(pipe_out.getFd(), buffer, sizeof(buffer) - 1)) > 0) 
            {
                buffer[bytesRead] = '\0';
                size_t offset = 0;
                ssize_t bytesWritten;
                while (offset < (size_t)bytesRead) 
                {
                    bytesWritten = ::write(fd_client, buffer + offset, bytesRead - offset);

                    if (bytesWritten == -1) 
                    {   
                        ::usleep(1000); 
                        continue;
                    } 
                    else if (bytesWritten == 0) 
                    {
                        ::perror("write returned 0 (unexpected)");
                        break;
                    }
                    offset += bytesWritten; 
                }
            }
            if (bytesRead == -1) 
                ::perror("read");
        }
        if (bytesRead == -1) 
            ::perror("read");
        bufferAccumulator2.clear();
        pipe_out.closeFd();
        pipe_in.removeFile();
        pipe_out.removeFile();
    }
    res.setStatusCode(200);
    Log::print_final_log("Response sent with status CGI:", 200, "FD:", fd_client);
}
