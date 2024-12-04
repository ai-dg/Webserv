/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipe.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:08 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/04 15:15:29 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../00-shared/includes.hpp"



class Pipe {
private:
    int fd;  // Descripteur pour lecture et écriture
    std::string tempFilePath;

public:
    Pipe();
    ~Pipe();

    int getReadFd() const;
    int getWriteFd() const;

    void write(const std::string &data);
    std::string read();

    void closeRead();
    void closeWrite();

    std::string getTempFilePath() const;
};