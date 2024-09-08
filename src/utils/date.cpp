/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   date.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ls <ls@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 21:34:06 by ls                #+#    #+#             */
/*   Updated: 2024/09/08 13:10:18 by ls               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include <cstring>
#include <string>
#include <sstream>
 
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
    dm << "date: " 
        << getDay(ltm->tm_wday) << ", " 
        << format_num(ltm->tm_mday) << " " 
        << getMonth(ltm->tm_mon) << " "
        << (1900 + ltm->tm_year) << " "
        << format_num(ltm->tm_hour) << ":"
        << format_num(ltm->tm_min) << ":"
        << format_num(ltm->tm_sec) << " GMT" ;
    std::string date = dm.str();
    return date;
}