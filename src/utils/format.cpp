/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ls <ls@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:10:05 by ls                #+#    #+#             */
/*   Updated: 2024/09/08 18:33:05 by ls               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/format.hpp"
#include <string>
#include <iostream>
#include <sstream>

std::string numberToString(int number) {
    std::ostringstream oss;
    oss << number;  // Convertir le nombre en chaîne
    return oss.str();
}

std::string getCrlf(void)
{
    std::string crlf = "  ";
    crlf[0] = 13;
    crlf[1] = 10;
    return (crlf);
}
