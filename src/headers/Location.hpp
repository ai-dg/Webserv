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
        std::string _index;
        int redirectionStatus;
        std::string redirectionPath;

        void extractField(std::string field);
    public :
        Location();
        Location(std::string &loc);
        void debugValues();
        Location &operator=(Location const &loc);
        void init();
        std::string redirection();
        std::string cgi();
        std::string cgi_bin();
        std::string upload_store();
        std::string autoindex();
        std::string methods();
        std::string extensions();
        std::string index();
        std::string root();
        int getRedirectionStatus();  
        void setRedirectionStatus();
        std::string getRedirectionPath();
        void setRedirectionPath();
        Location(Location const &loc);
        ~Location();
};


#endif