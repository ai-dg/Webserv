#include "../00-headers/01-core/scriptUtils.hpp"
#include "../00-headers/02-utils/stringUtils.hpp"

std::string getContextFromFile(std::string path)
{
    if (path[0] != '.')
        path = "." + path;
    std::ifstream file(path.c_str());
    std::string line = "";    
    std::string context = "";
    std::getline(file, line);
    line = trim(line);
 
    if (line.find("#!/bin/") != std::string::npos)
    {
        if (line.find("#!/bin/python") != std::string::npos)
            context = "python3";
        else if (line.find("#!/bin/bash") != std::string::npos)
        {
             context = "bash";
        }
        else if (line.find("#!/bin/perl") != std::string::npos)
            context = "perl";
        else if (line.find("#!") != std::string::npos)
            context = line.substr(line.find_last_of("/") + 1, std::string::npos);
    }
    if (line.find("<?php") != std::string::npos)
        context = "php";
    file.close();
    return context;
}