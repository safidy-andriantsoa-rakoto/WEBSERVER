/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:11:46 by barandri          #+#    #+#             */
/*   Updated: 2025/07/29 14:36:37 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	printFileContent(std::string const & path)
{
	std::string		line;
	std::ifstream	file;

	file.open(path.c_str());
	if (!file.is_open())
		return ;
	while (!file.eof())
	{
		std::getline(file, line);
		std::cout << line << std::endl;
	}
}

void    checkFileExtension(std::string const & fileName, std::string extension)
{
	if (fileName.compare(fileName.size() - extension.size(), extension.size(), extension) != 0)
		throw std::runtime_error("Extension file problem");
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

stringMap splitFileToMapString(std::string const &filePath, std::string const & sep)
{
	std::fstream	file;
	std::string		line;
	stringPair		pair;
	stringVect		vect;
	stringMap		result;

	file.open(filePath.c_str());
	if (!file.is_open())
		throw std::runtime_error("splitFileToMapString : File is not available");
	while (getline(file, line))
	{
		vect = strictSplit(line, sep);
		pair.first = vect[0];
		pair.second = vect[1];
		result.insert(pair);
		vect.clear();
		line.clear();
	}
	file.close();
	return (result);
}