/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directories.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:44 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 20:26:36 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/02-utils/directories.hpp"

bool pathIsDir(std::string dirPath)
{
    DIR *dir = opendir(dirPath.c_str());
    if (!dir)
    {
        return false;
    }
    closedir(dir);
    return (true);
}

bool doesFileExist(std::string filePath)
{
    if (access(filePath.c_str(), R_OK) == 0)
        return true;    
    return false; 
}
