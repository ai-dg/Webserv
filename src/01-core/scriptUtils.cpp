#include "../00-headers/01-core/scriptUtils.hpp"
#include "../00-headers/02-utils/stringUtils.hpp"

std::string getContextFromFile(std::string path)
{
    std::ifstream file(path.c_str());
    std::string line = "";    
    std::string context = "";
    std::getline(file, line);
    line = trim(line);
    if (line.find("#!/usr/bin/") != std::string::npos)
    {
        if (line.find("#!/usr/bin/python") != std::string::npos)
            context = "python3";
        if (line.find("#!/usr/bin/bash") != std::string::npos)
            context = "bash";
        if (line.find("#!/usr/bin/perl") != std::string::npos)
            context = "perl";
    }
    if (line.find("<?php"))
        context = "php";
    file.close();
    return context;
}