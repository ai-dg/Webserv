/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:36 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 20:25:41 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/02-utils/cleanup.hpp"
#include "../00-headers/02-utils/Log.hpp"

void clearMemory(std::vector<Conf *> Configs)
{
    Log::cleanup();
    clearArray(Configs);
}
