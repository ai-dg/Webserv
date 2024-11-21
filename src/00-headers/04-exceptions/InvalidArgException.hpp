/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InvalidArgException.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:57:41 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 18:57:43 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../00-shared/includes.hpp"

class InvalidArgException : public std::exception
{
    private :
        std::string message;
    public :
        InvalidArgException();
        InvalidArgException(std::string msg);
        const char *what() const throw();
        ~InvalidArgException() throw();
};
