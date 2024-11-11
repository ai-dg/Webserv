#include "../headers/Location.hpp"
#include "../headers/colors.hpp"
#include "../headers/stringUtils.hpp"
#include "../headers/directories.hpp"
#include  <sstream>



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
    

    int Location::getRedirectionStatus()
    {
        return redirectionStatus;
    }

    void Location::setRedirectionStatus()
    {
        redirectionStatus = stoi(_return.substr(0,_return.find(" ")).c_str());
    }

    std::string Location::getRedirectionPath()
    {
        return redirectionPath;
    }

    void Location::setRedirectionPath()
    {
        redirectionPath = _return.substr(_return.find(" ") + 1, std::string::npos);
    }

    void Location::setAllowedIndexes()
    {
        std::string cpy = trim(_index);
        if (_index == "")
            return ;
        size_t pos = cpy.find(" \t");
        if (pos == std::string::npos && cpy.size() > 0)
        {
            indexes.push_back(cpy);
            return ;
        }
        while (pos != std::string::npos)
        {       
            indexes.push_back(cpy.substr(0, pos));
            cpy.erase(0, pos + 1);
            pos = cpy.find(" \t");
            if (pos == std::string::npos && cpy.size() > 0)
                indexes.push_back(cpy);

        }
        
    }

    std::string Location::findIndex()
    {
        std::vector<std::string>::iterator it;
        it = indexes.begin();
        for (; it != indexes.end(); ++it)
        {
            if (doesFileExist("./" +_root + "/" + *it))
                return *it;
        }
        return "";
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
        {
            _index = value;
            setAllowedIndexes();
        }
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
        {
            _return = value;
            setRedirectionStatus();
            setRedirectionPath();
        }
        
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
        redirectionPath = loc.redirectionPath;
        redirectionStatus = loc.redirectionStatus;
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
        redirectionPath = "";
        redirectionStatus = 0;
    }


    Location::Location(Location const &loc)
    {
        *this = loc;
    }
    Location::~Location(){}

