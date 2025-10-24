/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:54:02 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/10/24 13:18:48 by rsafidy-         ###   ########.fr       */
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

std::string Mime::getFileType(std::string const &file)
{
    std::string extFile;
    stringMapIt it;
    std::string type;

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
        if (line.empty())
            continue;
        int pos = line.find_first_of("|");
        if (pos == -1)
            throw std::runtime_error("Mime file error: missing |");
        std::string ext = line.substr(pos + 1, line.size());
        if (ext.empty())
            throw std::runtime_error("Mime file error: empty extension");
        line.erase(pos, line.size());
        if (line.empty() || ext.empty())
            throw std::runtime_error("Mime file error: empty line or extension");
        pos = ext.find_first_of(",");
        if (pos != -1)
        {
            while (pos != -1)
            {
                std::string _ext = ext.substr(0, pos);
                if (_ext.empty())
                    throw std::runtime_error("Mime file error: empty extension before ,");
                _mime[_ext] = line;
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
    int             count = 0;

    DIR* dir = opendir(path.c_str());
	if (!dir)
        throw (std::runtime_error("No directory for Mime"));
    while ((entry = readdir(dir)) != NULL)
	{
        count++;
		std::string name(entry->d_name);
		if (!name.compare(".") || !name.compare(".."))
			continue;
        if (name != "app.conf" && name != "audio.conf" && name != "image.conf" && name != "text.conf" && name != "video.conf")
        {
            closedir(dir);
            throw std::runtime_error("ERROR: Invalid mime file in mime directory");
        }
		pathVect.push_back("./" + path + "/" + name);
    }
    closedir(dir);
    if (count != 7)
        throw std::runtime_error("ERROR: Missing mime file(s) or extra file(s) in mime directory");
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
    if (_mime.size() != 122)
        throw std::runtime_error("ERROR: Incorrect number of mime types loaded");
    for (stringMapIt it = _mime.begin(); it != _mime.end(); ++it)
    {
        if (it->first.empty() || it->second.empty())
            throw std::runtime_error("Mime file error: empty key or value in mime map");
        if (it->second.find("application/") == std::string::npos
            && it->second.find("audio/") == std::string::npos
            && it->second.find("video/") == std::string::npos
            && it->second.find("text/") == std::string::npos
            && it->second.find("image/") == std::string::npos)
            throw std::runtime_error("Mime file error: invalid mime type format");
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

