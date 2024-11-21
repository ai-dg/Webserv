/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InvalidArgException.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:59:09 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 18:59:10 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/04-exceptions/InvalidArgException.hpp"

InvalidArgException::InvalidArgException(): message ("Invalid argument in configuration file")
{}

InvalidArgException::InvalidArgException(std::string msg): message(msg)
{}

const char *InvalidArgException::what() const throw()
{
   return message.c_str();
}

InvalidArgException::~InvalidArgException() throw() {}