/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directories.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:44 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/05 05:30:17 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/02-utils/directories.hpp"
#include "../00-headers/02-utils/stringUtils.hpp"

bool pathIsDir(std::string dirPath)
{
    dirPath = removeDuplicateSlashes(dirPath);
    DIR *dir = opendir(dirPath.c_str());
    if (!dir)
    {
        return false;
    }
    closedir(dir);
    return (true);
}

bool isReadableFile(const std::string& path) 
{
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) == 0) 
    {
        if (S_ISREG(fileInfo.st_mode) && access(path.c_str(), R_OK) == 0) 
        {
            return true;
        }
    }
    return false;
}

bool doesFileExist(std::string filePath)
{
    if (pathIsDir(filePath.c_str()))
    {
        return false;
    }
    if (access(filePath.c_str(), R_OK) == 0)
        return true;    
    switch(errno) {
            case ENOENT: 
                std::cerr << ""; 
                break;
            case EACCES: 
                std::cerr << "";
                break;
            default: 
                std::cerr << "";
    }
        
    std::cerr << "";
    return false; 
}
