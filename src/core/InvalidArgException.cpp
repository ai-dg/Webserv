#include "../headers/InvalidArgException.hpp"


InvalidArgException::InvalidArgException(): message ("Invalid argument in configuration file")
{}

InvalidArgException::InvalidArgException(std::string msg): message(msg)
{}

const char *InvalidArgException::what() const throw()
{
   return message.c_str();
}

InvalidArgException::~InvalidArgException() throw() {}