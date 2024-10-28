#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include "includes.hpp"
#include <string>

std::string replaceBy(std::string original, std::string find, std::string replace);
std::pair<std::string, std::string> split(std::string const& str, char delimiter);
std::string itos(int number);
std::string upperCaseMe(std::string);
#endif