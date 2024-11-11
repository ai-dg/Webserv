#include "../headers/directories.hpp"
#include "../headers/colors.hpp"
#include <unistd.h>

bool pathIsDir(std::string dirPath)
{
    //std::cerr << BOLD_BLUE << "debug pathIsDir : - dirPath " << dirPath << "   - is Dir : ";
    DIR *dir = opendir(dirPath.c_str());
    if (!dir)
    {
       // std::cerr << RED << "false" << RESET << std::endl;
        return false;

    }
    //std::cerr << GREEN << "true" << RESET << std::endl;
    closedir(dir);
    return (true);
}

bool doesFileExist(std::string filePath)
{
    std::cerr << "file to test ::" << filePath << std::endl;
    if (access(filePath.c_str(), R_OK) == 0)
        return true;    
    std::cerr << "NONONONONONONONONO" << std::endl;
    return false; 
}