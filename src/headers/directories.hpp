#ifndef DIRECTORIES_HPP
#define DIRECTORIES_HPP

#include <iostream>
#include <dirent.h>

bool pathIsDir(std::string filePath);
bool doesFileExist(std::string filePath);

#endif