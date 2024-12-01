/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 13:17:17 by ls                #+#    #+#             */
/*   Updated: 2024/11/21 18:55:32 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_HPP
#define INCLUDES_HPP

/**
 * @brief General includes for the project
 */
#include "../00-shared/colors.hpp"
#include "../00-shared/defines.hpp"

/**
 * @brief Containerand STL includes
 */
#include <map>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <stack>

/**
 * @brief Time includes
 */
#include <ctime>

/**
 * @brief Stream includes
 */
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>

/**
 * @brief Network includes
 */
#include <sys/epoll.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * @brief System includes
 */
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <cstring>

/**
 * @brief Filesystem includes
 */
#include <dirent.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <zlib.h>
#include <sys/stat.h>

/**
 * @brief Exception includes
 */
#include <exception>
#include <stdexcept>

#endif