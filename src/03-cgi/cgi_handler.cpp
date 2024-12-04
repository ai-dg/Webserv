/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:59:06 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/05 00:40:30 by calbor-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/HttpRequest.hpp"
#include "../00-headers/01-core/scriptUtils.hpp"
#include "../00-headers/01-core/Pipe.hpp"
#include "../00-headers/02-utils/Log.hpp"
#include "../00-headers/02-utils/files.hpp"
#include "../00-headers/03-cgi/cgi_handler.hpp"

/**
 * @brief Prepare the environment for the CGI script
 */
std::string Cgi_handler::getExeContext(std::string file)
{
   // std::cerr << "get ExeContext debug file : " << file << std::endl;
    if (file.find(".") == std::string::npos)
        return "bash";
    if (file.find(".php") != std::string::npos)
        return "php-cgi";
    if (file.find(".py") != std::string::npos)
        return "python3.10";
    if (file.find(".pl") != std::string::npos)
        return "perl";
    if (file.find(".sh") != std::string::npos)
        return "bash";
    //if (file.find(".cgi") != std::string::npos) 
    return getContextFromFile(file);
    //return "";
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
    // std::ofstream file(filename.c_str());
    // file << body;
    // file.close();
    return filename;
    
}

void Cgi_handler::setEnvironment(HttpRequest &req)
{   
    std::map<std::string, std::string> headers = req.getHeaders();
    std::string requestMethodEnv = "REQUEST_METHOD=" + req.getMethod();
    std::string contentLengthEnv;

    // std::cerr << "Set method " << std::endl;
    if (req.getMethod() == "POST" || req.getMethod() == "DELETE")
    {
        // std::cerr << "Set content length " << std::endl;
        if (req.getHeader("Content-Length").size() != 0)
        {
            // std::cerr << "Set content length " << std::endl;
            contentLengthEnv =  "CONTENT_LENGTH=" + req.getHeader("Content-Length");
            
        }
        else if (req.getHeader("Content-Length").size() == 0 && req.getHeader("Transfer-Encoding") == "chunked")
        {
            // std::cerr << "Set content length 2" << std::endl;
            // std::cerr << req.getBody().length() << std::endl;

            // Conversion de la longueur du body en chaîne
            std::ostringstream oss;
            oss << req.getBody().length();
            contentLengthEnv = "CONTENT_LENGTH=" + oss.str();

            // std::cerr << "Content length set: " << contentLengthEnv << std::endl;
        }
         
        // std::cerr << "Set content length " << std::endl;
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

    // std::cerr << "Set headers " << std::endl;

    std::map<std::string, std::string>::iterator it;
    for (it = headers.begin(); it != headers.end(); ++it)
    {
        if (it->second == "chunked")
            continue;
        this->addToEnvironment(req.getFormatedHeader(it->first));  
    }

    // std::cerr << "Set other env " << std::endl;
    this->addToEnvironment("CONTENT_TYPE=" + req.getHeader("Content-Type"));
    this->addToEnvironment("REDIRECT_STATUS=1");
    this->addToEnvironment("SERVER_PROTOCOL=HTTP/1.1");

    
    this->addToEnvironment("PATH_INFO=/");
    
    if (getExeContext(scriptPath) == "php-cgi")
    {
        // ajouter php session ici
        this->addToEnvironment(req.getHeader("Cookie"));
        this->addToEnvironment("SCRIPT_NAME=" + scriptPath);
        this->addToEnvironment("SCRIPT_FILENAME=" + scriptPath);         
    }
    else if (getExeContext(scriptPath) == "python3")
        this->addToEnvironment("PYTHONWARNINGS=ignore");
    // if (req.getBody().size() > getFormatedSizeFromString("1M"))
    // {
        
    // }
    else
    {
        std::string filename = createBufferDataFile(req.getBody());
        this->addToEnvironment("CGI_FILE=" + filename);
        // this->addToEnvironment("CGI_BODY=" + req.getBody());
    }
    environment.push_back(NULL);
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
    // Log::output("./sessions/cgi_handler.txt") << "CGI Handler object class created" << std::endl;
}

Cgi_handler::Cgi_handler(Cgi_handler const& src)
{
    *this = src;
    // Log::output("./sessions/cgi_handler.txt") << "CGI Handler object class copied" << std::endl;
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
    // Log::output("./sessions/cgi_handler.txt") << "CGI Handler object class assigned" << std::endl;
    return *this;
}

Cgi_handler::~Cgi_handler()
{    
    for (size_t i = 0; i < environment.size(); ++i)
    {
        delete environment[i];
    }
    environment.clear();
    // Log::output("./sessions/cgi_handler.txt") << "CGI Handler object class destroyed" << std::endl;
}

/**
 * @brief Execute the CGI script
 */


void Cgi_handler::executeCGI(std::string const& scriptPath, HttpRequest &req, int fd_client) {
    pid_t pid;
    Pipe pipe_in("./sessions/pipe_infile");
    Pipe pipe_out("./sessions/pipe_outfile");
    this->scriptPath = scriptPath;
    std::string data = req.getBody();

    // std::cerr << "Entering executeCGI" << std::endl;

    if (pipe_in.getFd() == -1 || pipe_out.getFd() == -1) {
        std::cerr << "Error: Failed to open temporary files for Pipe." << std::endl;
        return;
    }

    // std::cerr << "Forking..." << std::endl;
    pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {  
        // std::cerr << "Child process started." << std::endl;

        ::lseek(pipe_in.getFd(), 0, SEEK_SET);

        if (dup2(pipe_in.getFd(), STDIN_FILENO) == -1) {
            perror("dup2 stdin");
            exit(1);
        }

        if (dup2(pipe_out.getFd(), STDOUT_FILENO) == -1) {
            perror("dup2 stdout");
            exit(1);
        }

        pipe_in.closeFd();
        pipe_out.closeFd();

        setEnvironment(req);

        std::string scriptPathTemp = scriptPath;
        size_t queryPos = scriptPathTemp.find('?');
        if (queryPos != std::string::npos)
            scriptPathTemp = scriptPathTemp.substr(0, queryPos);

        // debugEnvironment();
 
        std::string exe_context = getExeContext(scriptPath);
        std::string path = "/usr/bin/env";

        if (req.hasFileSpecialRoute(scriptPathTemp))
        {
            Location *route = req.getRouteConf(getExtension(scriptPathTemp));
            if (!route)
                std::cerr << "unkown route" << std::endl;
            if (!route->exe().empty())
            {
               if (resolvePath(route->exe()))        
                    path.assign(resolvePath(route->exe()));
            }
            if (resolvePath(scriptPathTemp))           
                exe_context.assign(resolvePath(scriptPathTemp));
            scriptPathTemp.clear();            
        }
/*
        if (scriptPathTemp.find(".bla") != std::string::npos) {
            std::cerr << "scriptPathTemp : " << scriptPathTemp << std::endl;
           // path = "/home/dagudelo/Parcours/Webserv/tests/ubuntu_cgi_tester";
            std::string path2 = "./www/test_site/cgi-bin/ubuntu_cgi_tester";
            
            //path.assign(resolvePath(path2));
            exe_context.assign(resolvePath(scriptPathTemp));
            scriptPathTemp.clear();
        }*/

        char *const argv[] = {
            const_cast<char *>(path.c_str()),
            const_cast<char *>(exe_context.c_str()),
            const_cast<char *>(scriptPathTemp.c_str()),
            NULL};

        // std::cerr << "Child: Executing script with execve..." << std::endl;
        if (execve(argv[0], argv, environment.data()) == -1) {
            perror("execve");
            exit(1);
        }
    } else {  
        // std::cerr << "Parent process started." << std::endl;

        

        size_t offset = 0;
        ssize_t bytes_written;

        while (offset < data.size()) {
            bytes_written = ::write(pipe_in.getFd(), data.c_str() + offset, data.size() - offset);
            if (bytes_written == -1) {
                if (errno == EPIPE) {
                    std::cerr << "Error: Client disconnected." << std::endl;
                    break;
                }
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

        if (WIFEXITED(status)) {
            std::cerr << "Child exited with status: " << WEXITSTATUS(status) << std::endl;
        } else if (WIFSIGNALED(status)) {
            std::cerr << "Child terminated by signal: " << WTERMSIG(status) << std::endl;
        } else {
            std::cerr << "Child ended abnormally." << std::endl;
        }

        char buffer[4096];
        ssize_t bytesRead;

        ::lseek(pipe_out.getFd(), 0, SEEK_SET);

        // std::cerr << "Script path: " << scriptPath << std::endl;

        std::string bufferAccumulator;
        std::string bufferAccumulator2;

        if (scriptPath.find(".bla") != std::string::npos) {
            // std::cerr << "Reading from bla file..." << std::endl;

            std::ostringstream headers;
            headers << "HTTP/1.1 200 OK\r\n"
                    << "Content-Length: " << data.size() << "\r\n"
                    << "Content-Type: text/html; charset=utf-8\r\n"
                    << "Date: Wed, 04 Dec 2024 16:22:54 GMT\r\n"
                    << "Server: webserv/1.0\r\n\r\n";

            std::string headersStr = headers.str();

            
            size_t offset = 0;
            ssize_t bytesWritten;

            while (offset < headersStr.size()) {
                bytesWritten = ::write(fd_client, headersStr.c_str() + offset, headersStr.size() - offset);
                if (bytesWritten == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        ::usleep(1000);
                        continue;
                    }
                    ::perror("write to client");
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

                
                if (!headersSkipped) {
                    size_t headerEnd = remainingBuffer.find("\r\n\r\n");
                    if (headerEnd != std::string::npos) {
                        headersSkipped = true;
                        
                        remainingBuffer = remainingBuffer.substr(headerEnd + 4);
                    } else {
                        
                        continue;
                    }
                }

                
                offset = 0;
                while (offset < remainingBuffer.size()) {
                    bytesWritten = ::write(fd_client, remainingBuffer.c_str() + offset, remainingBuffer.size() - offset);
                    if (bytesWritten == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            ::usleep(1000);
                            continue;
                        }
                        perror("write to client");
                        break;
                    }
                    offset += bytesWritten;
                }
                bufferAccumulator2 += remainingBuffer; 
                remainingBuffer.clear();               
            }

            if (bytesRead == -1) {
                perror("read");
            }

            if (data.size() >= 100000 && data.size() <= 200000)
            {
                
                std::ofstream debugFile("./sessions/debug_output.txt", std::ios::out | std::ios::trunc);
                if (debugFile.is_open()) {
                    debugFile << bufferAccumulator2;
                    debugFile.close();
                    // exit(1);
                } else {
                    std::cerr << "Error: Unable to open debug_output.txt for writing." << std::endl;
                }
                
            }
        }


        else 
        {
            while ((bytesRead = ::read(pipe_out.getFd(), buffer, sizeof(buffer) - 1)) > 0) {
                buffer[bytesRead] = '\0';

                size_t offset = 0;
                ssize_t bytesWritten;

                while (offset < (size_t)bytesRead) {
                    bytesWritten = ::write(fd_client, buffer + offset, bytesRead - offset);
                    if (bytesWritten == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            
                            usleep(1000);
                            continue;
                        }
                        perror("write to client");
                        break;
                    }
                    offset += bytesWritten;
                }
            }

            if (bytesRead == -1) {
                perror("read");
            }
        }

      

        if (bytesRead == -1) {
            perror("read");
        }

        // if (data.size() >= 100000 && data.size() <= 200000) {
        //     std::cerr << "Test 100000: Writing bufferAccumulator to file..." << std::endl;

        //     std::ofstream file("./sessions/100000.txt", std::ios::out | std::ios::trunc);
        //     if (!file.is_open()) {
        //         perror("Error opening 100000.txt");
        //         return;
        //     }

        //     // Écrire le contenu de bufferAccumulator dans le fichier
        //     file << bufferAccumulator2;

        //     if (file.fail()) {
        //         std::cerr << "Error writing to file 100000.txt" << std::endl;
        //     } else {
        //         std::cerr << "Data successfully written to ./sessions/100000.txt" << std::endl;
        //     }

        //     file.close();

        //     exit(1);
        // }


      
        bufferAccumulator2.clear();

        pipe_out.closeFd();

        pipe_in.removeFile();
        pipe_out.removeFile();
    }

    // std::cerr << "Exiting executeCGI" << std::endl;
}



// void Cgi_handler::executeCGI(std::string const& scriptPath, HttpRequest &req, int fd_client) {
//     pid_t pid;
//     Pipe pipe_in("./sessions/pipe_infile");
//     Pipe pipe_out("./sessions/pipe_outfile");
//     this->scriptPath = scriptPath;
//     std::string data = req.getBody();

//     std::cerr << "Entering executeCGI" << std::endl;

//     // Vérifier si les fichiers sont correctement ouverts
//     if (pipe_in.getFd() == -1 || pipe_out.getFd() == -1) {
//         std::cerr << "Error: Failed to open temporary files for Pipe." << std::endl;
//         return;
//     }

//     std::cerr << "Forking..." << std::endl;
//     pid = fork();
//     if (pid < 0) {
//         perror("fork");
//         return;
//     }

//     if (pid == 0) {  // Child process
//         std::cerr << "Child process started." << std::endl;

//         // Repositionner le pointeur de lecture au début du fichier temporaire
//         lseek(pipe_in.getFd(), 0, SEEK_SET);

//         // Duplication des fichiers pour stdin et stdout
//         std::cerr << "Child: Dup2 stdin with fd: " << pipe_in.getFd() << std::endl;
//         if (dup2(pipe_in.getFd(), STDIN_FILENO) == -1) {
//             perror("dup2 stdin");
//             exit(1);
//         }

//         std::cerr << "Child: Dup2 stdout with fd: " << pipe_out.getFd() << std::endl;
//         if (dup2(pipe_out.getFd(), STDOUT_FILENO) == -1) {
//             perror("dup2 stdout");
//             exit(1);
//         }

//         setEnvironment(req);

//         std::string scriptPathTemp = scriptPath;
//         size_t queryPos = scriptPathTemp.find('?');
//         if (queryPos != std::string::npos)
//             scriptPathTemp = scriptPathTemp.substr(0, queryPos);

//         debugEnvironment();

//         std::string exe_context = getExeContext(scriptPath);
//         std::string path = "/usr/bin/env";

//         if (scriptPathTemp.find(".bla") != std::string::npos) {
//             path = "/home/dagudelo/Parcours/Webserv/tests/ubuntu_cgi_tester";
//             exe_context = "/home/dagudelo/Parcours/find/webserv/www/YoupiBanane/youpi.bla";
//             scriptPathTemp.clear();
//         }

//         char *const argv[] = {
//             const_cast<char *>(path.c_str()),
//             const_cast<char *>(exe_context.c_str()),
//             const_cast<char *>(scriptPathTemp.c_str()),
//             NULL};

//         std::cerr << "Child: Executing script..." << std::endl;
//         if (execve(argv[0], argv, environment.data()) == -1) {
//             perror("execve");
//             exit(1);
//         }
        
//         std::cerr << "Child: Script executed." << std::endl;
        
//     } else {  // Parent process
//         std::cerr << "Parent process started." << std::endl;

//         // Écrire les données dans le fichier d'entrée
//         size_t offset = 0;
//         ssize_t bytes_written;

//         while (offset < data.size()) {
//             bytes_written = ::write(pipe_in.getFd(), data.c_str() + offset, data.size() - offset);
//             if (bytes_written == -1) {
//                 perror("write");
//                 pipe_in.closeFd();
//                 pipe_out.closeFd();
//                 return;
//             }
//             offset += bytes_written;
//         }

//         pipe_in.closeFd(); // Fermer l'écriture une fois terminé

//         // Attendre la fin du processus enfant
//         int status;
//         pid_t wpid  = waitpid(pid, &status, 0);
//         if (wpid == -1) 
//         {
//             perror("waitpid");
//             // Log::output("./logs/error.log") << "Parent: Failed to wait for child process." << std::endl;
//         } 
//         else 
//         {
//             if (WIFEXITED(status)) 
//                 // Log::output("./sessions/cgi_handler.txt") << "Parent: Child exited with status: " << WEXITSTATUS(status) << std::endl;
//             else if (WIFSIGNALED(status)) 
//                 // Log::output("./sessions/cgi_handler.txt") << "Parent: Child killed by signal: " << WTERMSIG(status) << std::endl;
//             else 
//                 // Log::output("./sessions/cgi_handler.txt") << "Parent: Child ended abnormally" << std::endl;
//         }

//         // Lecture des résultats depuis le fichier de sortie
//         char buffer[4096];
//         ssize_t bytesRead;

//         lseek(pipe_out.getFd(), 0, SEEK_SET); // Repositionner le pointeur pour la lecture

//         while ((bytesRead = ::read(pipe_out.getFd(), buffer, sizeof(buffer) - 1)) > 0) {
//             buffer[bytesRead] = '\0';
//             write(fd_client, buffer, bytesRead);
//         }
        
//         std::cerr << "Parent: Wrote " << bytesRead << " bytes to client." << std::endl;

//         if (bytesRead == -1) {
//             perror("read");
//         }

//         pipe_out.closeFd(); // Fermer la lecture une fois terminé
//     }

//     std::cerr << "Exiting executeCGI" << std::endl;
// }



// size_t offset = 0;
//         ssize_t bytes_written;
//         const size_t chunk_size = 16 * 1024; 
//         while (offset < data.size()) 
//         {
//             size_t to_write = std::min(chunk_size, data.size() - offset);
//             bytes_written = write(pipe_in[1], data.c_str() + offset, to_write);

//             if (bytes_written == -1) 
//             {
//                 if (errno == EAGAIN) 
//                 {
//                     std::cerr << "Parent: Pipe buffer full, retrying..." << std::endl;
//                     usleep(1000); // Wait briefly before retrying
//                     continue;
//                 }
//                 perror("write");
//                 close(pipe_in[1]);
//                 close(pipe_out[0]);
//                 return;
//             }

//             offset += bytes_written;
//             std::cerr << "Parent: Wrote " << bytes_written << " bytes to pipe." << std::endl;
//         }




/**
 * @brief Ancien CGI handler
 */

// void Cgi_handler::executeCGI(std::string const& scriptPath, HttpRequest &req, int fd_client) 
// {
//     pid_t pid;
//     int pipe_in[2]; 
//     int pipe_out[2];
//     this->scriptPath = scriptPath;
//     std::string data = req.getBody();

//     // // Log::output("./sessions/cgi_handler.txt") << data  << std::endl;
//     // // Log::output("./sessions/cgi_handler.txt") << BOLD_RED << req.getHeader("Content-Type") <<  RESET << std::endl;

//     std::cerr << "Entering executeCGI" << std::endl;

//     if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) 
//     {
//         perror("pipe");
//         return;
//     }
//     std::cerr << "Fork" << std::endl;
//     pid = fork();
//     if (pid < 0) 
//     {
//         perror("fork");
//         return;
//     }
//     std::cerr  << "pid : " << pid << std::endl;
//     if (pid == 0) 
//     { 
//         std::cerr << "Child process" << std::endl;
//         close(pipe_in[1]);  
//         close(pipe_out[0]);
//         std::cerr << "Dup2" << std::endl;
//         if (dup2(pipe_in[0], STDIN_FILENO) == -1) {
//             perror("dup2 stdin");
//             exit(1);
//         }
//         std::cerr << "Dup2" << std::endl;
//         if (dup2(pipe_out[1], STDOUT_FILENO) == -1) {
//             perror("dup2 stdout");
//             exit(1);
//         }
//         std::cerr << "Set environment" << std::endl;
//         setEnvironment(req);      
        
//         std::cerr << "Get exe context" << std::endl;
//         std::string scriptPathTemp = scriptPath;
//         size_t queryPos = scriptPathTemp.find('?');
//         if (queryPos != std::string::npos)
//             scriptPathTemp = scriptPathTemp.substr(0, queryPos);
//         debugEnvironment();   
//         std::string exe_context = getExeContext(scriptPath);
//         std::cerr << "executeCGI :: debug exe_context : " << exe_context << std::endl;
//         //std::cerr << "executeCGI :: debug exe_context : " << exe_context << std::endl;
    

//         std::string path = "/usr/bin/env";

//         if (scriptPathTemp.find(".bla") != std::string::npos)
//         {
//             path = "/home/dagudelo/Parcours/Webserv/tests/ubuntu_cgi_tester";
//             exe_context = "/home/dagudelo/Parcours/find/webserv/www/YoupiBanane/youpi.bla";
//             scriptPathTemp.clear();
//         }
//         else
//         {
//             path = "/usr/bin/env";
//         } 

//         char* const argv[] = {
//             const_cast<char*>(path.c_str()),  
//             const_cast<char*>(exe_context.c_str()),       
//             const_cast<char*>(scriptPathTemp.c_str()), 
//             NULL
//         };
        
//         std::cerr << BOLD_BLUE <<scriptPathTemp <<RESET << std::endl;
       
//         // if (access(scriptPathTemp.c_str(), X_OK) == -1)
//         // {
//         //     perror("acces");
//         //     // // Log::output("./logs/error.log") << "Child: Failed to access script file." << std::endl;
//         //     exit(1);
//         // }
//         std::cerr << "scriptPath child: " << scriptPathTemp << std::endl;

//         for (size_t i = 0; argv[i]; ++i)
//         {
//             std::cerr << BLUE << "argv[" << i << "] : " << argv[i] << RESET << std::endl;
//         }


//         if (execve(argv[0], argv, environment.data()) == -1)
//         {
//             perror("execve");
//             exit(1);
//         }     
//         perror("execve");
//         exit(1);
//     } 
//     else 
//     {
//         std::cerr << "Parent process" << std::endl;
        
        
//         close(pipe_in[0]);  
//         close(pipe_out[1]); 
//         std::ofstream outfile("./logs/data_cgi.log");
//         if (!data.empty()) 
//              write(pipe_in[1], data.c_str(), data.size());
//         outfile << data;
//         outfile.close();
//         close(pipe_in[1]); 
//         // Log::output("./sessions/cgi_handler.txt") << "Parent waiting..." << std::endl;
//         int status;
//         pid_t wpid = waitpid(pid, &status, 0);
//         if (wpid == -1) 
//         {
//             perror("waitpid");
//             // Log::output("./logs/error.log") << "Parent: Failed to wait for child process." << std::endl;
//         } 
//         else 
//         {
//             if (WIFEXITED(status)) 
//                 // Log::output("./sessions/cgi_handler.txt") << "Parent: Child exited with status: " << WEXITSTATUS(status) << std::endl;
//             else if (WIFSIGNALED(status)) 
//                 // Log::output("./sessions/cgi_handler.txt") << "Parent: Child killed by signal: " << WTERMSIG(status) << std::endl;
//             else 
//                 // Log::output("./sessions/cgi_handler.txt") << "Parent: Child ended abnormally" << std::endl;
//         }
        
//         char buffer[2048];
//         bzero(buffer, 2048);
//         int bytesRead = 0;
//         std::string context = getExeContext(scriptPath);
//         std::cerr << "context : " << context << std::endl;
//         if (context == "php-cgi" || context =="perl" || context =="bash")
//         {
//             std::string res = "HTTP/1.1 200 OK\r\n";
//             write(fd_client, res.c_str(), res.size());
//         }
//         // Log::output("./sessions/cgi_handler.txt") << "Parent: Reading from pipe to get script output..." << std::endl;
//         while ((bytesRead = read(pipe_out[0], buffer, sizeof(buffer) - 1)) > 0) 
//         {   
//             // std::cerr << buffer << std::endl;
//             // Log::output("./sessions/fd_client_cgi.txt") << buffer << std::endl;
//             write(fd_client, buffer, bytesRead);
//             bzero(buffer, 2048);
//             write(fd_client, "\r\n", 2);
//         }
//         if (bytesRead == -1) 
//         {
//             Log::error("read from pipe");
//             // Log::output("./logs/error.log") << "Parent: Failed to read from pipe." << std::endl;
//         }
//        // write(fd_client, "\r\n\r\n", 4);
//         close(pipe_out[0]); 
        

//     }    

    
    

//     std::cerr << "Exiting executeCGI" << std::endl;
// }
