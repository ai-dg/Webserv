#ifndef INVALIDARGEXCEPTION_HPP
#define INVALIDARGEXCEPTION_HPP

#include <exception>
#include <iostream>
#include <string>

class InvalidArgException : public std::exception
{
    private :
        std::string message;
    public :
        InvalidArgException();
        InvalidArgException(std::string msg);
        const char *what() const throw();
        ~InvalidArgException() throw();
};

#endif