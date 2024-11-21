/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:04 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 19:38:07 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/index.hpp"

std::string getHtmlHeaders()
{
   return std::string("<!DOCTYPE html>\n") +
            "<html lang=\"fr\">\n" +
                "\t<head>\n"  + 
                    "\t\t<meta charset=\"UTF-8\">\n" +
                    "\t\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">" +
                    "\t\t<link href=\"styles.css\" rel=\"stylesheet\">" +
                    "\t\t<title>My Webpage</title>" +
                "\t</head>" +
            "<body>";
}

std::string getHtmlFooter()
{
    return std::string("\t</body>\n</html>");
}

std::string getFormatedHtmlIndexLine(std::string path, char *name)
{
    (void) path;
    std::string filename(name);
    return "<a href=\"" + filename + "\">" + filename + "</a><br>\n";
}

std::string getIndexFile(std::string path)
{
    std::string html_index = getHtmlHeaders();
    DIR *dir = opendir(path.c_str());
    if (dir == NULL)
    {
        std::cerr << "can't access directory" << std::endl;
        return "";
    }
    struct dirent * files = readdir(dir);
    while (files)
    {
        std::string file(files->d_name);
        if (file != ".")
            html_index += getFormatedHtmlIndexLine(path, files->d_name);
        files = readdir(dir);
    }
    closedir(dir);
    html_index += getHtmlFooter();
    return html_index;
}
