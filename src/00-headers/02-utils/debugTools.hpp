/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugTools.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:56:26 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/04 21:35:38 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../00-shared/includes.hpp"
#include "../02-utils/Log.hpp"

template<typename T>
void printContenerValues(T & array)
{
    typename T::iterator it;
    for (it = array.begin(); it != array.end(); ++it)
    {
        Log::output("./sessions/Server.txt") << *it << std::endl;
    }
}

template<typename T>
void printContenerValues(T & array, std::string Color)
{
    typename T::iterator it;
    for (it = array.begin(); it != array.end(); ++it)
    {
        Log::output("./sessions/Server.txt") << Color << *it << RESET << std::endl;
    }
}
