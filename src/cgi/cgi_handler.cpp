#include "../headers/cgi_handler.hpp"
#include "../headers/HttpRequest.hpp"
#include <iostream>
#include <ostream>
#include <sys/wait.h>   
#include <cstdlib>      
#include <cstring>      
#include <cstdio>
#include <map>
#include "../headers/Log.hpp"
#include "../headers/colors.hpp"
#include <sstream>
#include <cstring>


Cgi_handler::Cgi_handler()
{
    Log::output("./sessions/cgi_handler.txt") << "CGI Handler created" << std::endl;
}

void Cgi_handler::addToEnvironment(const char * env)
{
    if (env)
        environment.push_back(strdup(const_cast<char *>(env)));
    else
        environment.push_back(NULL);
}

void Cgi_handler::addToEnvironment(std::string env)
{
    environment.push_back(strdup(const_cast<char*>(env.c_str())));
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

Cgi_handler::~Cgi_handler()
{    
    for (size_t i = 0; i < environment.size(); ++i)
    {
        delete environment[i];
    }
    environment.clear();
    Log::output("./sessions/cgi_handler.txt") << "CGI Handler destroyed" << std::endl;
}

std::string Cgi_handler::getExeContext(std::string file)
{
    if (file.find(".") == std::string::npos)
        return "bash";
    if (file.find(".php") != std::string::npos)
        return "php-cgi";
    if (file.find(".py") != std::string::npos)
        return "python3";
    if (file.find(".pl") != std::string::npos)
        return "perl";
    if (file.find(".sh") != std::string::npos)
        return "bash";
    return "";
}

void Cgi_handler::setEnvironment(HttpRequest &req)
{
    std::map<std::string, std::string> headers = req.getHeaders();
    std::string requestMethodEnv = "REQUEST_METHOD=" + req.getMethod();
    std::string contentLengthEnv;

    if (req.getMethod() == "POST") 
    {
        contentLengthEnv =  "CONTENT_LENGTH=" + req.getHeader("Content-Length");
    }
    else
        contentLengthEnv = ""; 

    addToEnvironment(requestMethodEnv);
    addToEnvironment(contentLengthEnv[0] ? const_cast<char*>(contentLengthEnv.c_str()) : NULL);
        
    std::map<std::string, std::string>::iterator it;
    for (it = headers.begin(); it != headers.end(); ++it)
    {
        addToEnvironment(req.getFormatedHeader(it->first));
    }      
    addToEnvironment("CONTENT_TYPE="+req.getHeader("Content-Type"));
        addToEnvironment("REDIRECT_STATUS=1");
    if (getExeContext(scriptPath) == "php-cgi")
    {
        
        addToEnvironment("SCRIPT_NAME=" + scriptPath);
        addToEnvironment("SCRIPT_FILENAME=" + scriptPath);         
    }else
    {
        addToEnvironment("PYTHONWARNINGS=ignore");
    }
        addToEnvironment(NULL);   

    Log::output("./sessions/cgi_handler.txt") << "Child: Environment variables set: " << requestMethodEnv
            << ", " << contentLengthEnv << std::endl;
}

void Cgi_handler::executeCGI(std::string const& scriptPath, HttpRequest &req, int fd_client) 
{
    pid_t pid;
    int pipe_in[2]; 
    int pipe_out[2];

    std::string data = req.getBody();

    Log::output("./sessions/cgi_handler.txt") << data  << std::endl;
    Log::output("./sessions/cgi_handler.txt") << BOLD_RED << req.getHeader("Content-Type") <<  RESET << std::endl;
 
    if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) 
    {
        perror("pipe");
        return;
    }
    
    pid = fork();
    if (pid < 0) 
    {
        perror("fork");
        return;
    }

    if (pid == 0) 
    { 
        close(pipe_in[1]);  
        close(pipe_out[0]); 
        
        if (dup2(pipe_in[0], STDIN_FILENO) == -1) {
            perror("dup2 stdin");
            exit(1);
        }
        if (dup2(pipe_out[1], STDOUT_FILENO) == -1) {
            perror("dup2 stdout");
            exit(1);
        }

        setEnvironment(req);
        // debugEnvironment();
 
        char* const argv[] = {
            const_cast<char*>("/usr/bin/env"),  
            const_cast<char*>(getExeContext(scriptPath).c_str()),       
            const_cast<char*>(scriptPath.c_str()), 
            NULL
        };

        //std::cerr << "Child: About to execute script using /usr/bin/env: " << scriptPath << std::endl;

        //
        execve("/usr/bin/env", argv, environment.data());     
        perror("execve");
        std::cerr << "Child: Failed to execute script: " << scriptPath << std::endl;
        exit(1);
        
    } 
    else 
    { 
        close(pipe_in[0]);  
        close(pipe_out[1]); 

        std::ofstream outfile("./logs/data_cgi.log");
        
        if (!data.empty()) 
        {
            write(pipe_in[1], data.c_str(), data.size());
        }
        outfile << data;
        outfile.close();

        close(pipe_in[1]); 
        
        Log::output("./sessions/cgi_handler.txt") << "Parent waiting..." << std::endl;
        int status;
        pid_t wpid = waitpid(pid, &status, 0);
        if (wpid == -1) 
        {
            perror("waitpid");
            Log::output("./logs/error.log") << "Parent: Failed to wait for child process." << std::endl;
        } 
        else 
        {
            if (WIFEXITED(status)) 
            {
                Log::output("./sessions/cgi_handler.txt") << "Parent: Child exited with status: " << WEXITSTATUS(status) << std::endl;
            } 
            else if (WIFSIGNALED(status)) 
            {
                Log::output("./sessions/cgi_handler.txt") << "Parent: Child killed by signal: " << WTERMSIG(status) << std::endl;
            } 
            else 
            {
                Log::output("./sessions/cgi_handler.txt") << "Parent: Child ended abnormally" << std::endl;
            }
        }
        
        char buffer[2048];
        bzero(buffer, 2048);
        int bytesRead = 0;
        std::string context = getExeContext(scriptPath);
        if (context == "php-cgi" || context =="perl" || context =="bash")
        {
            std::string res = "HTTP/1.1 200 OK\r\n";
            write(fd_client, res.c_str(), res.size());
        }
        Log::output("./sessions/cgi_handler.txt") << "Parent: Reading from pipe to get script output..." << std::endl;
        while ((bytesRead = read(pipe_out[0], buffer, sizeof(buffer) - 1)) > 0) 
        {
            //std::cerr << BOLD_RED << "CGI Output:" << std::string(buffer, bytesRead) << RESET << std::endl;
            //std::cerr << "Parent: Read " << bytesRead << " bytes from the pipe." << std::endl;
            //std::cerr << "-----------fd_client content---------" << std::endl;
            write(fd_client, buffer, bytesRead);
            //std::cerr << "-----------end fd_client ---------" << std::endl;
            // std::cerr << "----------   stdout   ------------" << std::endl;
            // write(STDIN_FILENO, buffer, bytesRead); 
            // std::cerr << "------------   end  --------------" << std::endl;
            bzero(buffer, 2048);
        }

        if (bytesRead == -1) 
        {
            Log::error("read from pipe");
            Log::output("./logs/error.log") << "Parent: Failed to read from pipe." << std::endl;
        }
        close(pipe_out[0]); 
    }
}
