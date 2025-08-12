/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:54:02 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/12 11:43:06 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mime.hpp"

Mime::Mime(stringVect &path)
{
    std::ifstream fd;
    for (size_t i = 0; i < path.size(); i++)
    {
        fd.open(path[i].c_str());
        if (fd.is_open())
        {
            this->get_file_content_mime(fd);
            std::cout << "Open" << std::endl;
            fd.close();
        }
    }
}

void Mime::trim_space(std::string &data)
{
    while (-1 != static_cast<int>(data.find(' ')))
        data.erase(data.find(' '), 1);
}

void Mime::get_file_content_mime(std::ifstream &fd)
{
    std::string line;
    std::cout << "---------------------" << std::endl;
    while (getline(fd, line))
    {
        this->trim_space(line);
        int pos = line.find_first_of("|");
        std::string ext = line.substr(pos + 1, line.size());
        line.erase(pos + 1, line.size());
        pos = ext.find_first_of(",");
        if (pos != -1)
        {
            
            std::cout << "Mila parse" << std::endl;
        }
        else
        {
            std::cout << "-->" << ext << std::endl;
            stringVect mime_txt;
            mime_txt.push_back(line);
            this->_mime[ext] = mime_txt;
        }
        // std::cout << pos << ":" << ext << " ->" << line << std::endl;
    }
}

Mime::~Mime() {}

Mime &Mime::operator=(const Mime &mime)
{
    this->_mime = mime._mime;
    return (*this);
}
