/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   date.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ls <ls@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 21:34:06 by ls                #+#    #+#             */
/*   Updated: 2024/09/08 10:45:40 by ls               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include <cstring>
#include <string>
#include <sstream>
 
std::string rec_num(int nb)
{
    std::ostringstream nbr;
    
    if (nb < 10)
        nbr << "0";
    nbr << nb;
    return nbr.str();
}

std::string get_current_date()
{
    std::string days[] = {"Sun", "Mon", "Thu", "Wed", "Thu", "Fri", "Sat"};
    std::string months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Agu", "Sep", "Oct", "Nov", "Dec"};
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::ostringstream dm;
    dm << "date: " << days[ltm->tm_wday] << ", " << rec_num(ltm->tm_mday) << " " << months[ltm->tm_mon] << " " <<
    (1900 + ltm->tm_year) << " " << rec_num(ltm->tm_hour) << ":" << rec_num(ltm->tm_min) << ":" << rec_num(ltm->tm_sec) << " GMT" ;
    std::string date = dm.str();
    return date;
}