/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:27 by barandri          #+#    #+#             */
/*   Updated: 2025/07/28 21:04:41 by barandri         ###   ########.fr       */
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

void    checkExtension(std::string const & fileName, std::string extension)
{
	if (fileName.compare(fileName.size() - extension.size(), extension.size(), extension) != 0)
		throw std::runtime_error("Extension file sent to cgi error");
}

void	displayFormatting(std::string const & text, std::string const & title)
{
	std::cout
	<< "=======================" << title << "=======================" << std::endl
	<< text 
	<< "===========End of " << title << "==============" << std::endl << std::endl;
}

void	displayStringPair(stringPair const & pair)
{
	std::cout << "[" << pair.first << "]: " << "[" << pair.second << "]" << std::endl;
}

void	displayStringMap(stringMap const & map)
{
	std::for_each(map.begin(), map.end(), displayStringPair);
}

stringVect	strictSplit(std::string const & target, std::string const & sep)
{
	size_t		pos;
	stringVect	result;

	pos = target.find_first_of(sep[0]);
	if (pos == std::string::npos)
		throw std::runtime_error("Strict split error 1");
	result.push_back(target.substr(0, pos));
	pos = target.find_first_of(sep[1], pos);
	if (pos == std::string::npos)
		throw std::runtime_error("Strict split error 2");
	pos++;
	result.push_back(target.substr(pos, target.size() - pos));
    return result;
}

stringVect	splitString(std::string const & target, std::string const & sep)
{
	size_t		findPos;
	size_t		startPos;
	stringVect	result;

	startPos = 0;
	findPos = 0;
	while (findPos != std::string::npos)
	{
		findPos = target.find(sep, startPos);
		result.push_back(target.substr(startPos, findPos - startPos));
		startPos = findPos + sep.size();
	}
	return result;
}

void split_line(std::vector<std::string> &_data, std::string line)
{
	size_t pos = -1;
	pos =  line.find_first_of(" ");
	while (pos != size_t(-1))
	{
		_data.push_back(line.substr(0, pos));
		line.erase(0, pos + 1);
		pos =  line.find_first_of(" ");
	}
	if (line.size())
	_data.push_back(line);
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

std::string response_generator(std::string response, std::string type, std::string content)
{
    std::stringstream out;
    out << response;// misy ilay zavatra nangatahin'ilay client
    out << "Content-Type: " << type << "\r\nAccess-Control-Allow-Origin: *\r\n";// mime text/html
    out << "Content-Length: " << content.size() << "\n\r\n";// index.html -> string -> omena ny size 
    out << content << "\r\n";
    return (out.str());
}

void send_value(int &_acceptStatus, std::string &_send_content)
{
    size_t tb = 0;
    while (tb < _send_content.size())
    {
        size_t _send_request = send(_acceptStatus, _send_content.c_str(), _send_content.size(), 0);
        if (!_send_request)
            std::cout << "Error\nSend" << std::endl;
        tb += _send_request;
    }
}
