/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:59:06 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 20:49:30 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/HttpRequest.hpp"
#include "../00-headers/02-utils/Log.hpp"
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
        return "python3";
    if (file.find(".pl") != std::string::npos)
        return "perl";
    if (file.find(".sh") != std::string::npos)
        return "bash";
    return "";
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

void Cgi_handler::setEnvironment(HttpRequest &req)
{
    std::map<std::string, std::string> headers = req.getHeaders();
    std::string requestMethodEnv = "REQUEST_METHOD=" + req.getMethod();
    std::string contentLengthEnv;

    if (req.getMethod() == "POST" || req.getMethod() == "DELETE")
        contentLengthEnv =  "CONTENT_LENGTH=" + req.getHeader("Content-Length");
    else
        contentLengthEnv = ""; 
    addToEnvironment(requestMethodEnv);
    addToEnvironment(contentLengthEnv[0] ? const_cast<char*>(contentLengthEnv.c_str()) : NULL);
    
    size_t queryPos = scriptPath.find('?');
    if (queryPos != std::string::npos) {
        std::string queryString = scriptPath.substr(queryPos + 1);
        addToEnvironment("QUERY_STRING=" + queryString);
    }

    std::map<std::string, std::string>::iterator it;
    for (it = headers.begin(); it != headers.end(); ++it)
        addToEnvironment(req.getFormatedHeader(it->first));
    addToEnvironment("CONTENT_TYPE="+req.getHeader("Content-Type"));
    addToEnvironment("REDIRECT_STATUS=1");
    if (getExeContext(scriptPath) == "php-cgi")
    {
        addToEnvironment("SCRIPT_NAME=" + scriptPath);
        addToEnvironment("SCRIPT_FILENAME=" + scriptPath);         
    }
    else
        addToEnvironment("PYTHONWARNINGS=ignore");
    addToEnvironment(NULL);   

    Log::output("./sessions/cgi_handler.txt") << "Child: Environment variables set: " << requestMethodEnv
            << ", " << contentLengthEnv << std::endl;
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
    Log::output("./sessions/cgi_handler.txt") << "CGI Handler object class created" << std::endl;
}

Cgi_handler::Cgi_handler(Cgi_handler const& src)
{
    *this = src;
    Log::output("./sessions/cgi_handler.txt") << "CGI Handler object class copied" << std::endl;
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
    Log::output("./sessions/cgi_handler.txt") << "CGI Handler object class assigned" << std::endl;
    return *this;
}

Cgi_handler::~Cgi_handler()
{    
    for (size_t i = 0; i < environment.size(); ++i)
    {
        delete environment[i];
    }
    environment.clear();
    Log::output("./sessions/cgi_handler.txt") << "CGI Handler object class destroyed" << std::endl;
}

/**
 * @brief Execute the CGI script
 */

void Cgi_handler::executeCGI(std::string const& scriptPath, HttpRequest &req, int fd_client) 
{
    pid_t pid;
    int pipe_in[2]; 
    int pipe_out[2];
    this->scriptPath = scriptPath;
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
        std::string scriptPathTemp = scriptPath;
        size_t queryPos = scriptPathTemp.find('?');
        if (queryPos != std::string::npos)
            scriptPathTemp = scriptPathTemp.substr(0, queryPos);
            
        std::string exe_context = getExeContext(scriptPath);
        char* const argv[] = {
            const_cast<char*>("/usr/bin/env"),  
            const_cast<char*>(exe_context.c_str()),       
            const_cast<char*>(scriptPathTemp.c_str()), 
            NULL
        };
        execve("/usr/bin/env", argv, environment.data());     
        perror("execve");
        exit(1);
    } 
    else 
    { 
        close(pipe_in[0]);  
        close(pipe_out[1]); 
        std::ofstream outfile("./logs/data_cgi.log");
        if (!data.empty()) 
            write(pipe_in[1], data.c_str(), data.size());
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
                Log::output("./sessions/cgi_handler.txt") << "Parent: Child exited with status: " << WEXITSTATUS(status) << std::endl;
            else if (WIFSIGNALED(status)) 
                Log::output("./sessions/cgi_handler.txt") << "Parent: Child killed by signal: " << WTERMSIG(status) << std::endl;
            else 
                Log::output("./sessions/cgi_handler.txt") << "Parent: Child ended abnormally" << std::endl;
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
            write(fd_client, buffer, bytesRead);
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
