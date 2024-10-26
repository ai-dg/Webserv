#include "../headers/cgi_handler.hpp"
#include <iostream>
#include <sys/wait.h>   
#include <cstdlib>      
#include <cstring>      
#include <cstdio>
#include "../headers/Log.hpp"
#include <sstream>

Cgi_handler::Cgi_handler()
{
    std::cout << "CGI Handler created" << std::endl;
}

Cgi_handler::~Cgi_handler()
{
    std::cout << "CGI Handler destroyed" << std::endl;
}

void Cgi_handler::executeCGIWithoutFork(const std::string& scriptPath, const std::string& queryString, int fd_client) 
{
    std::cout << "Execute without Fork" << std::endl;

    char requestMethodEnv[] = "REQUEST_METHOD=POST";    
    char queryStringEnv[256];
    
    std::stringstream queryStringStream;
    queryStringStream << "QUERY_STRING=" << queryString;
    strncpy(queryStringEnv, queryStringStream.str().c_str(), sizeof(queryStringEnv) - 1);
    queryStringEnv[sizeof(queryStringEnv) - 1] = '\0';  
    char* const envp[] = {requestMethodEnv, queryStringEnv, NULL};

    std::cerr << "Environment variables set: REQUEST_METHOD=" << requestMethodEnv
              << ", QUERY_STRING=" << queryStringEnv << std::endl;

    char* const argv[] = {
        const_cast<char*>("/usr/bin/python3"), 
        const_cast<char*>(scriptPath.c_str()), 
        NULL
    };

    std::cerr << "About to execute script using execve" << std::endl;
    execve("/usr/bin/python3", argv, envp);
    perror("execve");
    std::cerr << "Failed to execute script: " << scriptPath << std::endl;
    exit(1);
}
/*
void Cgi_handler::executeCGI(std::string const& scriptPath, const std::string& data, const std::string& method, int fd_client) 
{
    pid_t pid;
    int pipe_in[2];  
    int pipe_out[2]; 

    std::cout << "Fonction script..." << std::endl;
    std::cout << "Method: " << method << std::endl;
    std::cout << "Data: " << data << std::endl;  
 
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

        char requestMethodEnv[256];
        std::stringstream requestMethodStream;
        requestMethodStream << "REQUEST_METHOD=" << method;
        strncpy(requestMethodEnv, requestMethodStream.str().c_str(), sizeof(requestMethodEnv) - 1);
        requestMethodEnv[sizeof(requestMethodEnv) - 1] = '\0';

        char contentLengthEnv[256];
        if (method == "POST") 
        {
            std::stringstream contentLengthStream;
            contentLengthStream << "CONTENT_LENGTH=" << data.size();
            strncpy(contentLengthEnv, contentLengthStream.str().c_str(), sizeof(contentLengthEnv) - 1);
            contentLengthEnv[sizeof(contentLengthEnv) - 1] = '\0';
        } else {
            contentLengthEnv[0] = '\0'; 
        }

        char pythonWarningsEnv[] = "PYTHONWARNINGS=ignore";        
        char* const envp[] = {requestMethodEnv, contentLengthEnv[0] ? contentLengthEnv : NULL, pythonWarningsEnv, NULL};

        std::cerr << "Child: Environment variables set: REQUEST_METHOD=" << requestMethodEnv
                  << ", CONTENT_LENGTH=" << contentLengthEnv << std::endl;
        
        char* const argv[] = {
            const_cast<char*>("/usr/bin/env"),  
            const_cast<char*>("python3"),       
            const_cast<char*>(scriptPath.c_str()), 
            NULL
        };

        std::cerr << "Child: About to execute script using /usr/bin/env: " << scriptPath << std::endl;

        execve("/usr/bin/env", argv, envp);     
        perror("execve");
        std::cerr << "Child: Failed to execute script: " << scriptPath << std::endl;
        exit(1);
    } 
    else 
    { 
        close(pipe_in[0]);  
        close(pipe_out[1]); 
        
        if (!data.empty()) 
        {
            write(pipe_in[1], data.c_str(), data.size());
        }
        close(pipe_in[1]); 
        
        std::cout << "Parent waiting..." << std::endl;
        int status;
        pid_t wpid = waitpid(pid, &status, 0);
        if (wpid == -1) 
        {
            perror("waitpid");
            std::cerr << "Parent: Failed to wait for child process." << std::endl;
        } 
        else 
        {
            if (WIFEXITED(status)) 
            {
                std::cerr << "Parent: Child exited with status: " << WEXITSTATUS(status) << std::endl;
            } 
            else if (WIFSIGNALED(status)) 
            {
                std::cerr << "Parent: Child killed by signal: " << WTERMSIG(status) << std::endl;
            } 
            else 
            {
                std::cerr << "Parent: Child ended abnormally" << std::endl;
            }
        }
        
        char buffer[2048];
        bzero(buffer, 2048);
        int bytesRead = 0;

        std::cerr << "Parent: Reading from pipe to get script output..." << std::endl;
        while ((bytesRead = read(pipe_out[0], buffer, sizeof(buffer) - 1)) > 0) 
        {
            std::cerr << "Parent: Read " << bytesRead << " bytes from the pipe." << std::endl;
            write(fd_client, buffer, bytesRead);
            std::cout << "-----------fd_client content---------" << std::endl;
            write(STDIN_FILENO, buffer, bytesRead); 
            std::cout << "-------------------------------------" << std::endl;
            bzero(buffer, 2048);
        }

        if (bytesRead == -1) 
        {
            perror("read from pipe");
            std::cerr << "Parent: Failed to read from pipe." << std::endl;
        }

        close(pipe_out[0]); 
    }
}*/


void Cgi_handler::executeCGI(std::string const& scriptPath, const std::string& data, const std::string& method, int fd_client) 
{
    pid_t pid;
    int pipe_in[2];  
    int pipe_out[2]; 

    std::cout << "Fonction script..." << std::endl;
    std::cout << "Method: " << method << std::endl;
    std::cout << "Data: " << data << std::endl;  
 
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

        std::string requestMethodEnv = "REQUEST_METHOD=" + method;
        std::string contentLengthEnv = "" ;//= itos(data.size());

        if (method == "POST") 
        {
            contentLengthEnv =  "CONTENT_LENGTH=" + itos(data.size());
            Log::debug(contentLengthEnv);
            //std::cout << data.size() << " str :: " << itos(data.size()) << std::endl;
        }
        else
            contentLengthEnv = ""; 

        char pythonWarningsEnv[] = "PYTHONWARNINGS=ignore";        
        char* const envp[] = {const_cast<char *>(requestMethodEnv.c_str()), 
            contentLengthEnv[0] ? const_cast<char *>(contentLengthEnv.c_str()) : NULL, 
            pythonWarningsEnv, 
            NULL};

        std::cerr << "Child: Environment variables set: REQUEST_METHOD=" << requestMethodEnv
                  << ", CONTENT_LENGTH=" << contentLengthEnv << std::endl;
        
        char* const argv[] = {
            const_cast<char*>("/usr/bin/env"),  
            const_cast<char*>("python3"),       
            const_cast<char*>(scriptPath.c_str()), 
            NULL
        };

        std::cerr << "Child: About to execute script using /usr/bin/env: " << scriptPath << std::endl;

        execve("/usr/bin/env", argv, envp);     
        perror("execve");
        std::cerr << "Child: Failed to execute script: " << scriptPath << std::endl;
        exit(1);
    } 
    else 
    { 
        close(pipe_in[0]);  
        close(pipe_out[1]); 
        
        if (!data.empty()) 
        {
            write(pipe_in[1], data.c_str(), data.size());
        }
        close(pipe_in[1]); 
        
        std::cout << "Parent waiting..." << std::endl;
        int status;
        pid_t wpid = waitpid(pid, &status, 0);
        if (wpid == -1) 
        {
            perror("waitpid");
            std::cerr << "Parent: Failed to wait for child process." << std::endl;
        } 
        else 
        {
            if (WIFEXITED(status)) 
            {
                std::cerr << "Parent: Child exited with status: " << WEXITSTATUS(status) << std::endl;
            } 
            else if (WIFSIGNALED(status)) 
            {
                std::cerr << "Parent: Child killed by signal: " << WTERMSIG(status) << std::endl;
            } 
            else 
            {
                std::cerr << "Parent: Child ended abnormally" << std::endl;
            }
        }
        
        char buffer[2048];
        bzero(buffer, 2048);
        int bytesRead = 0;

        std::cerr << "Parent: Reading from pipe to get script output..." << std::endl;
        while ((bytesRead = read(pipe_out[0], buffer, sizeof(buffer) - 1)) > 0) 
        {
            std::cerr << "Parent: Read " << bytesRead << " bytes from the pipe." << std::endl;
            write(fd_client, buffer, bytesRead);
            std::cout << "-----------fd_client content---------" << std::endl;
            write(STDIN_FILENO, buffer, bytesRead); 
            std::cout << "-------------------------------------" << std::endl;
            bzero(buffer, 2048);
        }

        if (bytesRead == -1) 
        {
            perror("read from pipe");
            std::cerr << "Parent: Failed to read from pipe." << std::endl;
        }

        close(pipe_out[0]); 
    }
}
