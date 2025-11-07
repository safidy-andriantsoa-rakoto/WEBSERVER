/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:11:46 by barandri          #+#    #+#             */
/*   Updated: 2025/11/07 20:02:59 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

bool	checkFileExtension(std::string const & fileName, std::string extension)
{
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

stringVect getContentFile(std::string const & _path)
{
	std::fstream	fluxFile;
	stringVect		lines;

	fluxFile.open(_path.c_str(), std::ios::in);
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
		std::cerr << "Error : to open the file" << std::endl;
	return (lines);
}

int	openRegFile(std::string const & filePath)
{
	int			fd;
	struct stat	st;

	if (stat(filePath.c_str(), &st) == -1)
	{
		if (errno == EACCES)
			throw std::runtime_error("Forbidden");
		throw std::runtime_error("Other");
	}
	if (S_ISREG(st.st_mode) == false)
		throw std::runtime_error("Other");
	fd = open(filePath.c_str(), O_RDONLY);
	if (fd == -1)
	{
		if (errno == EPERM || errno == EACCES)
			throw std::runtime_error("Forbidden");
		throw std::runtime_error("Not found");
	}
	return fd;
}

size_t	getFileSize(std::string const & path)
{
	struct stat	sf;

	if (stat(path.c_str(), &sf) == -1)
		throw std::runtime_error("Stat function error");
	// std::cout << "\e[0;35mFile size = " << sf.st_size  << "\e[0m" << std::endl;
	return sf.st_size;
}

DIR*	openDir(std::string const & dirPath)
{
	DIR	*dir;

	dir = opendir(dirPath.c_str());
	if (!dir)
		throw std::runtime_error("Opendir error");
	return dir;
}

bool	checkIfDirectoryAvailable(std::string const & path)
{
	struct stat	st;

	if (stat(path.c_str(), &st) == -1)
		return false;
	if (S_ISDIR(st.st_mode) == false)
		return false;
	return true;
}

bool	checkIfFileAvailabe(std::string const & path)
{
	struct stat	st;

	if (stat(path.c_str(), &st) == -1)
		return false;
	if (S_ISREG(st.st_mode) == false)
		return false;
	return true;
}