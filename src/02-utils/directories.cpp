/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directories.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:44 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/04 21:40:10 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/02-utils/directories.hpp"
#include "../00-headers/02-utils/stringUtils.hpp"

bool pathIsDir(std::string dirPath)
{
    dirPath = removeDuplicateSlashes(dirPath);
    // std::cerr << "PATHISDIR - " << dirPath << " - ";
    DIR *dir = opendir(dirPath.c_str());
    if (!dir)
    {
        // std::cerr << "false" << std::endl;
        return false;
    }
    closedir(dir);
    // std::cerr << "true" << std::endl;
    return (true);
}

bool isReadableFile(const std::string& path) {
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) == 0) {
        if (S_ISREG(fileInfo.st_mode) && access(path.c_str(), R_OK) == 0) {
            return true; // C'est un fichier lisible
        }
    }
    return false;
}

bool doesFileExist(std::string filePath)
{
    // std::cerr << "try !!!!!!!!!!!!!!!" << std::endl;
    if (pathIsDir(filePath.c_str()))
    {
        // std::cerr << "it's a directory !!!!!!!!!!!!!!!" << std::endl;
        return false;
    }
    if (access(filePath.c_str(), R_OK) == 0)
        return true;    
    switch(errno) {
            case ENOENT: 
                std::cerr << "Le fichier n'existe pas" << std::endl; 
                break;
            case EACCES: 
                std::cerr << "Permissions insuffisantes" << std::endl; 
                break;
            default: 
                std::cerr << "Erreur inconnue" << std::endl;
    }
        
    std::cerr << "can't open !!!!!!!!!!!!!!!" << std::endl;
    return false; 
}
