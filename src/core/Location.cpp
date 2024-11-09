#include "../headers/Location.hpp"
#include "../headers/colors.hpp"
#include "../headers/stringUtils.hpp"



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

    void Location::debugValues()
    {
        if (_root != "")
            std::cerr << BOLD_WHITE << _root << RESET << std::endl;
        if (_extensions != "")
            std::cerr << BOLD_WHITE << _extensions << RESET << std::endl;
        if (_methods != "")
            std::cerr << BOLD_WHITE << _methods << RESET << std::endl;
        if (_autoindex != "")
            std::cerr << BOLD_WHITE << _autoindex << RESET << std::endl;
        if (_upload_store != "")
              std::cerr << BOLD_WHITE << _upload_store << RESET << std::endl;
        if (_cgi_bin != "")
            std::cerr << BOLD_WHITE << _cgi_bin << RESET << std::endl;
        if (_cgi != "")
            std::cerr << BOLD_WHITE << _cgi << RESET << std::endl;
        if (_return != "")
            std::cerr << BOLD_WHITE << _return << RESET << std::endl;
    }

    void Location::extractField(std::string param)
    {
        std::string field;
        std::string value;

        value = param.substr(param.find_first_of(" \t") + 1, std::string::npos);
        field = param.substr(0, param.find_first_of(" \t"));
        if (field == "root")
            _root = value;
        if (field == "extensions")
            _extensions = value;
        if (field == "methods")
            _methods = value;
        if (field == "autoindex")
            _autoindex = value;
        if (field == "upload_store")
            _upload_store = value;
        if (field == "cgi_bin")
            _cgi_bin = value;
        if (field == "cgi")
            _cgi = value;
        if (field == "return")
            _return = value;
        
        //std::cerr << BOLD_WHITE << "extracted :: " <<  field << " - " << value << RESET << std::endl;
    }

    Location::Location(std::string &loc)
    {
        (void) loc;
        std::string currentLine;
        init();
        while(loc.find("\n") != std::string::npos)
        {
            currentLine = loc.substr(0, loc.find_first_of("\n"));
            extractField(currentLine);
            loc.erase(0,loc.find_first_of("\n") + 1);
        }
        debugValues();
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

