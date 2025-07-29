/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:11:46 by barandri          #+#    #+#             */
/*   Updated: 2025/07/29 08:15:46 by barandri         ###   ########.fr       */
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
		throw std::runtime_error("Extension file sent to cgi error");
}

std::string readFileToString(std::string _path)
{
	std::fstream index;

	index.open(_path.c_str());
	if (!index.is_open())
		throw std::runtime_error("File can't be opened");
	std::string _index_value((std::istreambuf_iterator<char>(index)), std::istreambuf_iterator<char>());
	return (_index_value);
}