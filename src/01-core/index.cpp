/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calbor-p <calbor-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:04 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/27 18:36:26 by calbor-p         ###   ########.fr       */
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
                    "\t\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n" +
                    "\t\t<title>My Webpage</title>\n" +
                    "\t\t<style>\n" +
                        "\t\t\tbody {\n" +
                        "\t\t\t\tmargin: 0;\n" +
                        "\t\t\t\tpadding: 0;\n" +
                        "\t\t\t\tbackground-color: #f9f9f9;\n" +
                        "\t\t\t\tfont-family: Arial, sans-serif;\n" +
                        "\t\t\t\tcolor: #333;\n" +
                        "\t\t\t}\n\n" +
                        "\t\t\t.paragraph {\n" +
                        "\t\t\t\tcursor: pointer;\n" +
                        "\t\t\t\tdisplay: block;\n" +
                        "\t\t\t\tcolor:black;\n" +
                        "\t\t\t\tmargin-top: 3px;\n" +
                        "\t\t\t\tmargin-left: 50px;\n" +
                        "\t\t\t\tfont-weight: bold;\n" +
                        "\t\t\t\tfont-size: 20px;\n" +
                        "\t\t\t}\n\n" +
                        "\t\t\t.link {\n" +
                        "\t\t\t\tcursor: pointer;\n" +
                        "\t\t\t\ttext-decoration: none;\n" +
                        "\t\t\t\tdisplay: block;\n" +
                        "\t\t\t\tcolor:black;\n" +
                        "\t\t\t\tmargin-top: 3px;\n" +
                        "\t\t\t\tmargin-left: 100px;\n" +
                        "\t\t\t\tfont-weight: bold;\n" +
                        "\t\t\t\tfont-size: 16px;\n" +
                        "\t\t\t}\n\n" +
                        "\t\t\t.link:hover {\n" +
                        "\t\t\t\ttext-decoration: underline;\n" +
                        "\t\t\t\tfont-weight: bold;\n" +
                        "\t\t\t\tcolor: #d9534f;\n" +
                        "\t\t\t}\n\n" +                        
                    "\t\t</style>\n" +
                "\t</head>\n" +
            "\t<body>\n";
}


std::string getHtmlFooter()
{
    return std::string("\t</body>\n</html>");
}

std::string getFormatedHtmlIndexLine(std::string path, char const* name)
{
    (void) path;
    std::string filename(name);
    return "<a class=\"link\"href=\"" + filename + "\">" + filename + "</a>\n";
}

std::string getParentLink(std::string path)
{
    DIR *dir = ::opendir(path.c_str());
    if (dir == NULL)
    {
        std::cerr << "can't access directory" << std::endl;
        return "";
    }
    struct dirent * files = ::readdir(dir);
    while (files)
    {
        std::string file(files->d_name);
        if (file == "..")
        {
            ::closedir(dir);   
            return getFormatedHtmlIndexLine(path, file.c_str());
        }
        files = ::readdir(dir);
    }
    ::closedir(dir);
    return "";
}

std::string getFilesLinks(std::string path)
{
    std::string html_index;
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
        if (file != "." && file != "..")
            html_index += getFormatedHtmlIndexLine(path, files->d_name);
        files = readdir(dir);
    }
    closedir(dir);
    return html_index;
}

std::string getIndexFile(std::string path)
{
    std::string html_index = getHtmlHeaders();
    html_index += "<br><br><p class=\"paragraph\">Directory :</p>";
    html_index += getParentLink(path);
    html_index += getFilesLinks(path);
    html_index += getHtmlFooter();
    return html_index;
}
