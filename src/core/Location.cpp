#include "../headers/Location.hpp"
#include "../headers/colors.hpp"



  /* private:
        std::string _root;
        std::string _extensions ;
        std::string _methods ;
        std::string _autoindex ;
        std::string _upload_store ;
        std::string _cgi;
        std::string _cgi_bin;
        std::string _return;*/

    Location::Location()
    {
        init();
    }

    Location::Location(std::string &loc)
    {
        (void) loc;
        init();
        std::cerr << "called : ";
        /*ici la logique pour récupérer les différentes valeurs...*/
        std::cerr << VIOLET << loc << RESET << std::endl;
    }

    Location &Location::operator=(Location const &loc)
    {   
        _root= loc._root;
        _extensions = loc._extensions;
        _methods = loc._methods;
        _autoindex = loc._autoindex;
        _upload_store = loc._upload_store;
        _cgi= loc._cgi;
        _cgi_bin= loc._cgi_bin;
        _return= loc._return;    
        return *this;
    }

    void Location::init()
    {
        _root = "";
        _extensions = "";
        _methods = "";
        _autoindex = "";
        _upload_store = "";
        _cgi = "";
        _cgi_bin = "";
        _return = "";
    }


    Location::Location(Location const &loc)
    {
        *this = loc;
    }
    Location::~Location(){}

