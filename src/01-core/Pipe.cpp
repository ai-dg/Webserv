/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipe.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:08 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/09 09:11:53 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/Pipe.hpp"


Pipe::Pipe(std::string const& path) : path(path)
{
    fd = open(path.c_str(), O_CREAT | O_RDWR, 0644);
    if (fd == -1) 
    {
        ::close(fd);
        throw std::runtime_error("Failed to create/read temporary file for writeFd");
    }
}

Pipe::~Pipe() 
{
    if (fd != -1)
    {
        closeFd();
        fd = -1;
    }
    ::remove(path.c_str());
}

int Pipe::getFd() const 
{
    return fd;
}

void Pipe::closeFd() 
{
    if (fd != -1)
    {
       ::close(fd);
       fd = -1;  
    }
}

std::string Pipe::getPath() 
{
    return path;
}

void Pipe::removeFile() 
{
    remove(path.c_str());
}