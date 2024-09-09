/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ls <ls@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:23:00 by ls                #+#    #+#             */
/*   Updated: 2024/09/09 18:47:12 by ls               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/files.hpp"

std::string getFile(const std::string& path)
{
    std::ifstream file(path.c_str());
    std::string content;
    std::string line;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (!content.empty())
                content += "\n";
            content += line;
        }
        file.close();
    }
    return content;
}
