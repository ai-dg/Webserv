/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipe.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:08 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/05 05:10:14 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/Pipe.hpp"

Pipe::Pipe(std::string const& path) : path(path)
{
    fd = open(path.c_str(), O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        perror("open write file");
        ::close(fd);
        throw std::runtime_error("Failed to create/read temporary file for writeFd");
    }
}

Pipe::~Pipe() 
{
    closeFd();
    ::remove(path.c_str());
}

int Pipe::getFd() const 
{
    return fd;
}

void Pipe::closeFd() 
{
    ::close(fd);
}

std::string Pipe::getPath() 
{
    return path;
}

void Pipe::removeFile() 
{
    remove(path.c_str());
}