/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:11:46 by barandri          #+#    #+#             */
/*   Updated: 2025/11/15 15:23:29 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

bool	checkFileExtension(std::string const & fileName, std::string extension)
{
	if (fileName.find("..conf") != std::string::npos)
		return false;
	if (fileName.compare(fileName.size() - extension.size(), extension.size(), extension) != 0)
		return false;
	return true;
}

std::string readFileToString(std::string filePath)
{
	std::fstream file;

	file.open(filePath.c_str());
	if (!file.is_open())
		throw std::runtime_error("ReadFileToString : File can't be opened");
	std::string fileString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return (fileString);
}

std::string	readFromRegFileFd(int & fd, int & stat)
{
    char		buffer[_BUFFER + 1] = {0};
    int			bytesRead;

	bytesRead = read(fd, buffer, _BUFFER);
	if (bytesRead <= 0)
	{
		stat = OP_CLOSE;
		close(fd);
		fd = -1;
		if (bytesRead < 0)
			throw std::runtime_error("Error on reading");
	}
	buffer[bytesRead] = '\0';
	std::string res(buffer, bytesRead);
	if (stat == OP_ONCE)
	{
		close(fd);
		fd = -1;
	}
    return res;
}

std::string	getFileExtension(std::string const & fileName)
{
	std::string ext_file;
	int 		pe;

	ext_file = fileName;
	pe = fileName.find_last_of(".");
	ext_file.erase(0, pe + 1);
	return ext_file;
}

stringVect getContentFile(std::string const & path)
{
	std::fstream	fluxFile;
	stringVect		lines;

	fluxFile.open(path.c_str(), std::ios::in);
	if (fluxFile.is_open())
	{
		std::string line;
		while (std::getline(fluxFile, line))
		{
			line += "\n";
			lines.push_back(line);
		}
		fluxFile.close();
	}
	else
		throw std::runtime_error("The configuration file can't be reached");
	if (lines.size() == 0)
		throw std::runtime_error("The configuration file is empty");
	return (lines);
}

int	openRegFile(std::string const & filePath)
{
	int         fd;
    struct stat st;

    if (stat(filePath.c_str(), &st) == -1)
    {
        if (errno == ENOENT || errno == EACCES) 
            throw ErrorExcept(404, "Access denied or file doesn't exist");
		throw ErrorExcept(500, "Internal server error");
    }
    if (S_ISREG(st.st_mode) == false) 
        throw ErrorExcept(403, "Access denied or file doesn't exist");
    fd = open(filePath.c_str(), O_RDONLY);
    if (fd == -1)
    {
        if (errno == EPERM || errno == EACCES) 
            throw ErrorExcept(403, "Access denied or file doesn't exist");
		throw ErrorExcept(500, "Internal server error");
    }
    return fd;
}

size_t	getFileSize(std::string const & path)
{
	struct stat	sf;

	if (stat(path.c_str(), &sf) == -1)
		throw std::runtime_error("Stat function error");
	return sf.st_size;
}

bool	checkIfDirectoryAvailable(std::string const & path)
{
	DIR	*dir;
	struct stat	st;

	if (stat(path.c_str(), &st) == -1)
		return false;
	if (S_ISDIR(st.st_mode) == false)
		return false;
	dir = opendir(path.c_str());
	if (!dir)
		return false;
	closedir(dir);
	return true;
}

bool	checkIfFileAvailabe(std::string const & path, int const & flag)
{
	int         fd;
    struct stat st;

    if (stat(path.c_str(), &st) == -1)
		return false;
    if (S_ISREG(st.st_mode) == false) 
        return false;
    fd = open(path.c_str(), flag);
    if (fd == -1)
		return false;
    close(fd);
	return true;
}

bool	checkIfParentDirectory(std::string const & parentPath, std::string const & childPath)
{
	DIR 			*dir;
	struct dirent*	dirent;
	std::string		childName;
	int				stat;
	size_t			pos;

	std::cout << "Parent = " << parentPath << std::endl;
	std::cout << "Child = " << childPath << std::endl;
	dir = opendir(parentPath.c_str());
	if (!dir)
		return false;
	pos = childPath.find_last_of('/');
	if (pos != std::string::npos)
	{
		pos++;
		childName = childPath.substr(pos, childPath.size() - pos);
	}
	else
		childName = childPath;
	std::cout << "CHILD NAME = " << childName << std::endl;
	stat = false;
	while ((dirent = readdir(dir)))
	{
		if (dirent->d_name == childName)
		{
			stat = true;
			break;
		}
	}
	closedir(dir);
	return stat;
}