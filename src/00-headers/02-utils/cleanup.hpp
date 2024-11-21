/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:56:21 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 18:56:22 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../00-shared/includes.hpp"
#include "../01-core/Server.hpp"
#include "../01-core/Conf.hpp"

void clearMemory(std::vector<Conf *> Configs);

template<typename T>
void clearArray(std::vector<T*> &array)
{
    typename std::vector<T*>::iterator it;
    for (it = array.begin(); it != array.end(); ++it)
    {
        delete *it;
    }
    array.clear();
}

template<typename T, typename U>
void clearMaps(std::map<T, U*> &map)
{
    typename std::map<T, U*>::iterator it;
    for (it = map.begin(); it != map.end(); ++it)
    {
        delete it->second;
    }
    map.clear();
}
