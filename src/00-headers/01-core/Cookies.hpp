/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:55:40 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 18:55:41 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../00-shared/includes.hpp"

class Cookies
{
    private:
        std::map<std::string, std::string> cookies;

        /**
         * @brief Parse the cookie header and store the cookies in the map
         */
        void parseCookies(std::string const& cookieHeader);

    public:
        /**
         * @brief Coplien Form
         */
        Cookies(std::string const& cookieHeader);
        Cookies(Cookies const& src);
        Cookies& operator=(Cookies const& src);
        ~Cookies();

        /**
         * @brief Getters
         */
        std::string getCookie(std::string const& name);
        std::string getSetCookieHeader();

        /**
         * @brief Setters
         */
        void setCookie(std::string const& name, std::string const& value);
};
