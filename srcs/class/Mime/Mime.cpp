/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:54:02 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/14 14:33:53 by rsafidy-         ###   ########.fr       */
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
            fd.close();
        }
    }
}

void Mime::trim_space(std::string &data)
{
    while (-1 != static_cast<int>(data.find(' ')))
        data.erase(data.find(' '), 1);
}

void Mime::print_map()
{
    for (MapMime::iterator it = _mime.begin(); it != _mime.end(); ++it)
    {
        std::cout << "->" << it->first << std::endl;
        std::cout << "-->" << it->second << std::endl;
        std::cout << std::endl;
    }
}

MapMime Mime::get_mime() const
{
    return (this->_mime);
}

void Mime::get_file_content_mime(std::ifstream &fd)
{
    std::string line;
    while (getline(fd, line))
    {
        this->trim_space(line);
        int pos = line.find_first_of("|");
        std::string ext = line.substr(pos + 1, line.size());
        line.erase(pos, line.size());
        pos = ext.find_first_of(",");
        if (pos != -1)
        {
            while (pos != -1)
            {
                this->_mime[ext.substr(0, pos)] = line;
                ext.erase(0, pos + 1);
                pos = ext.find_first_of(",");
            }
            this->_mime[ext] = line;
        }
        else
            this->_mime[ext] = line;
    }
}

Mime::~Mime() {}

Mime &Mime::operator=(const Mime &mime)
{
    this->_mime = mime._mime;
    return (*this);
}
