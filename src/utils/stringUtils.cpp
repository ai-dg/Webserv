#include "../headers/stringUtils.hpp"
#include <sstream>
#include <string>
#include <map>

std::string replaceBy(std::string original, std::string find, std::string replace)
{
    int pos = original.find(find);
    original.erase(pos, find.size());
    original.insert(pos, replace);
    return original;
}

std::pair<std::string, std::string> split(std::string const& str, char delimiter) 
{
    size_t pos = str.find(delimiter);
    return std::pair<std::string, std::string>(str.substr(0, pos), str.substr(pos + 1));
}

std::string itos(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}

std::string upperCaseMe(std::string str)
{
    int i = 0;
    std::string cpy(str);

	while (cpy[i] != '\0')
	{
		cpy[i] = toupper(cpy[i]);
        i++;
	}
    return cpy;
}
