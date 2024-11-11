#include "../headers/directories.hpp"
#include "../headers/colors.hpp"


bool pathIsDir(std::string dirPath)
{
    std::cerr << BOLD_BLUE << "debug pathIsDir : - dirPath " << dirPath << "   - is Dir : ";
    DIR *dir = opendir(dirPath.c_str());
    if (!dir)
    {
        std::cerr << RED << "false" << RESET << std::endl;
        return false;

    }
    std::cerr << GREEN << "true" << RESET << std::endl;
    closedir(dir);
    return (true);
}