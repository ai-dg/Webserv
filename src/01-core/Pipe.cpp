/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipe.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:08 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/04 15:18:00 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/Pipe.hpp"


Pipe::Pipe() {
    char tempTemplate[] = "./sessions/pipe_sim_XXXXXX";

    
    fd = mkstemp(tempTemplate);
    if (fd == -1) {
        throw std::runtime_error("Failed to create temporary file for Pipe simulation");
    }

    
    tempFilePath = tempTemplate;

    
    unlink(tempTemplate);
}

Pipe::~Pipe() {
    if (fd != -1) {
        close(fd);
    }
}

int Pipe::getReadFd() const {
    return fd;
}

int Pipe::getWriteFd() const {
    return fd;
}

void Pipe::write(const std::string &data) {
    ssize_t bytesWritten = ::write(fd, data.c_str(), data.size());
    if (bytesWritten == -1) {
        throw std::runtime_error("Failed to write to Pipe");
    }
    std::cerr << "Pipe: Wrote " << bytesWritten << " bytes to file." << std::endl;

    
    fsync(fd);
}

std::string Pipe::read() {
    
    lseek(fd, 0, SEEK_SET);

    char buffer[4096];
    ssize_t bytesRead = ::read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead == -1) {
        throw std::runtime_error("Failed to read from Pipe");
    }
    buffer[bytesRead] = '\0';
    std::cerr << "Pipe: Read " << bytesRead << " bytes from file." << std::endl;

    return std::string(buffer);
}

void Pipe::closeRead() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

void Pipe::closeWrite() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

std::string Pipe::getTempFilePath() const {
    return tempFilePath;
}