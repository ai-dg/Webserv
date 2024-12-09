/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:56:18 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 18:56:19 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../00-shared/includes.hpp"

class Status
{
    private:
        static std::map<int, std::string> initializeCodes();
        static const std::map<int, std::string>codes;
    public:
        static std::string get(int code);
        static std::string get(std::string code);
};
