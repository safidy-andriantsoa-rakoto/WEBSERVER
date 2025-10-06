/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:54:02 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/25 22:11:51 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mime.hpp"

MapMime Mime::_mime;

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

/*
    What status code if file not supported, especially with POST request? [Safidy]
*/
std::string Mime::getFileType(std::string const &file)
{
    std::string extFile;
    stringMapIt it;
    std::string type;

	std::cout << "		BON TRAVAIL EEE" << std::endl;
    extFile = getFileExtension(file);
    it = _mime.find(extFile);
    if (extFile == "/")
    	type = "text/html";
    else if (it != _mime.end())
    	type = it->second;
    else
        throw ErrorExcept(404, "Error on extension");
    return type;
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
        trim_space(line);
        int pos = line.find_first_of("|");
        std::string ext = line.substr(pos + 1, line.size());
        line.erase(pos, line.size());
        pos = ext.find_first_of(",");
        if (pos != -1)
        {
            while (pos != -1)
            {
                _mime[ext.substr(0, pos)] = line;
                ext.erase(0, pos + 1);
                pos = ext.find_first_of(",");
            }
            _mime[ext] = line;
        }
        else
            _mime[ext] = line;
    }
}

Mime::~Mime() {}

void Mime::initMapMime(std::string const &path)
{
    stringVect      pathVect;
    std::ifstream   fileStream;
    struct dirent*  entry;

    DIR* dir = opendir(path.c_str());
	if (!dir)
        throw (std::runtime_error("No directory for Mime"));
    while ((entry = readdir(dir)) != NULL)
	{
		std::string name(entry->d_name);
		if (!name.compare(".") || !name.compare(".."))
			continue;
		pathVect.push_back("./" + path + "/" + name);
    }
    closedir(dir);
    for (size_t i = 0; i < pathVect.size(); i++)
    {
        fileStream.open(pathVect[i].c_str());
        if (fileStream.is_open())
        {
            get_file_content_mime(fileStream);
            fileStream.close();
        }
        else
            throw std::runtime_error("Mime file error");
    }
}

MapMime &Mime::getMapMime()
{
   return _mime;
}

Mime &Mime::operator=(const Mime &mime)
{
    this->_mime = mime._mime;
    return (*this);
}

