/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:10:05 by ls                #+#    #+#             */
/*   Updated: 2024/12/05 05:31:30 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/02-utils/format.hpp"

std::string numberToString(int number) {
    std::ostringstream oss;
    oss << number;
    return oss.str();
}

std::string getCrlf(void)
{
    std::string crlf = "  ";
    crlf[0] = 13;
    crlf[1] = 10;
    return (crlf);
}
