#ifndef COOKIES_HPP
#define COOKIES_HPP

#include <map>
#include <string>

class Cookies
{
    private:
        std::map<std::string, std::string> cookies;
        void parseCookies(std::string const& cookieHeader);

    public:
        Cookies(std::string const& cookieHeader);
        ~Cookies();

        std::string getCookie(std::string const& name);
        void setCookie(std::string const& name, std::string const& value);
        std::string getSetCookieHeader();
};

#endif