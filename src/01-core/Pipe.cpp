/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipe.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:08 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/04 21:38:16 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/Pipe.hpp"


Pipe::Pipe(std::string const& path) : path(path)
{
    fd = open(path.c_str(), O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        perror("open write file");
        close(fd);
        throw std::runtime_error("Failed to create/read temporary file for writeFd");
    }

    // std::cerr << "Pipe: file created: " << path << std::endl;
}

Pipe::~Pipe() {
    // closeRead();
    // closeWrite();

    // // Supprimer les fichiers temporaires
    // if (!readFilePath.empty()) remove(readFilePath.c_str());
    // if (!writeFilePath.empty()) remove(writeFilePath.c_str());
}


int Pipe::getFd() const {
    return fd;
}

void Pipe::closeFd() {
    close(fd);
}

std::string Pipe::getPath() {
    return path;
}


void Pipe::removeFile() {
    remove(path.c_str());
}