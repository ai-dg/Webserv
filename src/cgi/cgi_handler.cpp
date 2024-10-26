#include "../headers/cgi_handler.hpp"
#include <iostream>
#include <sys/wait.h>   
#include <cstdlib>      
#include <cstring>      
#include <cstdio>
#include "../headers/Log.hpp"
#include "../headers/colors.hpp"
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

std::string Cgi_handler::getExeContext(std::string file)
{
    if (file.find(".") == std::string::npos)
        return "bash";
    if (file.find(".php") != std::string::npos)
        return "php-cgi";
    if (file.find(".py") != std::string::npos)
        return "python3";
    if (file.find(".pl") != std::string::npos)
        return "pl";
    if (file.find(".pl") != std::string::npos)
        return "bash";
    return "";

}

void Cgi_handler::executeCGI(std::string const& scriptPath, const std::string& data, const std::string& method, int fd_client) 
{
    pid_t pid;
    int pipe_in[2]; 
    int pipe_out[2];

    std::cout << BOLD_VIOLET << scriptPath << RESET << std::endl;
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
        std::string contentLengthEnv;

        if (method == "POST") 
        {
            contentLengthEnv =  "CONTENT_LENGTH=" + itos(data.size());
        }
        else
            contentLengthEnv = ""; 

        
       /*if (getExeContext(scriptPath) == "php-cgi")
        {*/
            std::string scriptName = "SCRIPT_NAME=" + scriptPath;
            std::string scriptFilename = "SCRIPT_FILENAME=" + scriptPath;
            std::string contentType = "CONTENT_TYPE=application/x-www-form-urlencoded";
            std::string gatewayInterface = "GATEWAY_INTERFACE=CGI/1.1";
std::string serverProtocol = "SERVER_PROTOCOL=HTTP/1.1";
std::string serverSoftware = "SERVER_SOFTWARE=WebServ/1.0";
std::string documentRoot = "DOCUMENT_ROOT=/cgi-bin/"; //getDocumentRoot(); // Fonction à implémenter selon votre configuration
std::string phpSelf = "PHP_SELF=" + scriptPath;

    char* const envp[] = {
        const_cast<char*>(requestMethodEnv.c_str()),
        const_cast<char*>(contentType.c_str()),
        const_cast<char*>(scriptName.c_str()),
        const_cast<char*>(scriptFilename.c_str()),
        const_cast<char*>(gatewayInterface.c_str()),
        const_cast<char*>(serverProtocol.c_str()),
        const_cast<char*>(serverSoftware.c_str()),
        const_cast<char*>(documentRoot.c_str()),
        const_cast<char*>(phpSelf.c_str()),
        contentLengthEnv[0] ? const_cast<char*>(contentLengthEnv.c_str()) : NULL,
        const_cast<char*>("REDIRECT_STATUS=1"),
        NULL
    };
        

       /* }else
        {

            char pythonWarningsEnv[] = "PYTHONWARNINGS=ignore";        
            char* const envp[] = {const_cast<char *>(requestMethodEnv.c_str()), 
                contentLengthEnv[0] ? const_cast<char *>(contentLengthEnv.c_str()) : NULL, 
                pythonWarningsEnv, 
                const_cast<char *>("REDIRECT_STATUS=1"),
                NULL};
        }*/



        std::cerr << "Child: Environment variables set: REQUEST_METHOD=" << requestMethodEnv
                  << ", CONTENT_LENGTH=" << contentLengthEnv << std::endl;
 
        char* const argv[] = {
            const_cast<char*>("/usr/bin/env"),  
            const_cast<char*>(getExeContext(scriptPath).c_str()),       
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
        
        std::cerr << "Parent waiting..." << std::endl;
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
    
        if (getExeContext(scriptPath) == "php-cgi")
        {
            std::string res = "HTTP/1.1 200 OK\r\n";
            write(fd_client, res.c_str(), res.size());
        }
        std::cerr << "Parent: Reading from pipe to get script output..." << std::endl;
        while ((bytesRead = read(pipe_out[0], buffer, sizeof(buffer) - 1)) > 0) 
        {
            //std::cerr << BOLD_RED << "CGI Output:" << std::string(buffer, bytesRead) << RESET << std::endl;
            //std::cerr << "Parent: Read " << bytesRead << " bytes from the pipe." << std::endl;
            std::cerr << "-----------fd_client content---------" << std::endl;
            write(fd_client, buffer, bytesRead);
            std::cerr << "-----------end fd_client ---------" << std::endl;
            std::cerr << "----------   stdout   ------------" << std::endl;
            write(STDIN_FILENO, buffer, bytesRead); 
            std::cerr << "------------   end  --------------" << std::endl;
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
