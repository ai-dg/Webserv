#include "../headers/stringUtils.hpp"


std::string replaceBy(std::string original, std::string find, std::string replace)
{
    int pos = original.find(find);
    original.erase(pos, find.size());
    original.insert(pos, replace);
    return original;
}