/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:56:13 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 20:21:13 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../00-shared/includes.hpp"

extern int signalPipeFd[2];
extern volatile sig_atomic_t signalReceived;

void signalHandler(int signal);
void setupSignalHandler();

class SignalException : public std::exception
{
    public:
        virtual const char* what() const throw();
};
