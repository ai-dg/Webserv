#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <string>

class Location
{
    private:
        std::string _root;
        std::string _extensions ;
        std::string _methods ;
        std::string _autoindex ;
        std::string _upload_store ;
        std::string _cgi;
        std::string _cgi_bin;
        std::string _return;
    public : 
        Location();
        Location(std::string &loc);
        Location &operator=(Location const &loc);
        void init();
        Location(Location const &loc);
        ~Location();
};


#endif