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
        std::string _index;
        std::string _return;*/

    Location::Location()
    {
        init();
    }

    void Location::debugValues()
    {
        if (_root != "")
            std::cerr << "\t\t" << BOLD_WHITE << _root << RESET << std::endl;
        if (_extensions != "")
            std::cerr << "\t\t" << BOLD_WHITE << _extensions << RESET << std::endl;
        if (_methods != "")
            std::cerr << "\t\t" << BOLD_WHITE << _methods << RESET << std::endl;
        if (_autoindex != "")
            std::cerr << "\t\t" << BOLD_WHITE << _autoindex << RESET << std::endl;
        if (_upload_store != "")
              std::cerr << "\t\t" << BOLD_WHITE << _upload_store << RESET << std::endl;
        if (_cgi_bin != "")
            std::cerr << "\t\t" << BOLD_WHITE << _cgi_bin << RESET << std::endl;
        if (_cgi != "")
            std::cerr << "\t\t" << BOLD_WHITE << _cgi << RESET << std::endl;
        if (_return != "")
            std::cerr << "\t\t" << BOLD_WHITE << _return << RESET << std::endl;
        if (_index != "")
            std::cerr << "\t\t" << BOLD_WHITE << _index << RESET << std::endl;
    }
        std::string Location::index()
        {
            return _index;
        }
        std::string Location::redirection()
        {
            return _return;
        }
        std::string Location::cgi()
        {
            return _cgi;
        }
        std::string Location::cgi_bin()
        {
            return _cgi_bin;
        }
        std::string Location::upload_store()
        {
            return _upload_store;
        }
        std::string Location::autoindex()
        {
            return _autoindex;
        }
        std::string Location::methods()
        {
            return _methods;
        }
        std::string Location::extensions()
        {
            return _extensions;
        }
        std::string Location::root()
        {
            return _root;
        }

    void Location::extractField(std::string param)
    {
        std::string field;
        std::string value;

        value = param.substr(param.find_first_of(" \t") + 1, param.find_last_of(";") - param.find_first_of(" \t") - 1);
        field = param.substr(0, param.find_first_of(" \t"));
        if (field == "root")
            _root = value;
        if (field == "index")
            _index = value;
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
        //debugValues();
        //std::cerr << VIOLET << loc << RESET << std::endl;

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
        _index= loc._index; 
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
        _index = "";
    }


    Location::Location(Location const &loc)
    {
        *this = loc;
    }
    Location::~Location(){}

