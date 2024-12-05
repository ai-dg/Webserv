/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   date.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 21:34:06 by ls                #+#    #+#             */
/*   Updated: 2024/12/05 03:28:26 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/02-utils/format.hpp"
 
std::string format_num(int nb)
{
    std::ostringstream nbr;
    
    if (nb < 10)
        nbr << "0";
    nbr << nb;
    return nbr.str();
}

std::string getDay(int d)
{
    std::string days[] = {"Sun", "Mon", "Thu", "Wed", "Thu", "Fri", "Sat"};
    return (days[d]);
}

std::string getMonth(int m)
{
    std::string months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Agu", "Sep", "Oct", "Nov", "Dec"};
    return (months[m]);
}

std::string get_current_date()
{   
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::ostringstream dm;
    dm  << getDay(ltm->tm_wday) << ", " 
        << format_num(ltm->tm_mday) << " " 
        << getMonth(ltm->tm_mon) << " "
        << (1900 + ltm->tm_year) << " "
        << format_num(ltm->tm_hour) << ":"
        << format_num(ltm->tm_min) << ":"
        << format_num(ltm->tm_sec) << " GMT";
    std::string date = dm.str();
    return date;
}

std::string get_current_time()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::ostringstream dm;
    dm  << format_num(ltm->tm_hour) << ":"
        << format_num(ltm->tm_min) << ":"
        << format_num(ltm->tm_sec);
    std::string date = dm.str();
    return date;
}

bool checkTimeOut()
{
    return true;
}