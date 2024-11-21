/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:03:15 by calbor-p          #+#    #+#             */
/*   Updated: 2024/11/21 18:55:14 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_HPP
# define COLORS_HPP

/*
[4;XXm : souligné
[7;XXm : fond
[9;XXm : barré
*/
# define GREY "\033[30m"
# define BOLD_GREY "\033[1;30m"
# define UNDER_GREY "\033[4;30m"
# define BG_GREY "\033[40m"

# define RED "\033[31m"
# define BOLD_RED "\033[1;31m"
# define UNDER_RED "\033[4;31m"
# define BG_RED "\033[41m"

# define GREEN "\033[32m"
# define BOLD_GREEN "\033[1;32m"
# define UNDER_GREEN "\033[4;32m"
# define BG_GREEN "\033[42m"

# define YELLOW "\033[33m"
# define BOLD_YELLOW "\033[1;33m"
# define UNDER_YELLOW "\033[4;33m"
# define BG_YELLOW "\033[43m"

# define BLUE "\033[34m"
# define BOLD_BLUE "\033[1;34m"
# define UNDER_BLUE "\033[4;1;34m"
# define BG_BLUE "\033[44m"

# define VIOLET "\033[35m"
# define BOLD_VIOLET "\033[1;35m"
# define UNDER_VIOLET "\033[4;35m"
# define BG_VIOLET "\033[45m"

# define TURQUOISE "\033[36m"
# define BOLD_TURQUOISE "\033[1;36m"
# define UNDER_TURQUOISE "\033[4;36m"
# define BG_TURQUOISE "\033[46m"

# define WHITE "\033[37m"
# define BOLD_WHITE "\033[1;37m"
# define UNDER_WHITE "\033[4;37m"
# define BG_WHITE "\033[47m"

# define RESET "\033[0m"

# define BLINK "\033[5;35m"
#endif