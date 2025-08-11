/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stringUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:27 by barandri          #+#    #+#             */
/*   Updated: 2025/08/11 20:06:20 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

stringVect	strictSplit(std::string const & target, std::string const & sep)
{
	size_t		pos;
	stringVect	result;

	pos = target.find_first_of(sep);
	if (pos == std::string::npos)
		throw std::runtime_error("Strict split error 1");
	result.push_back(target.substr(0, pos));
	pos += sep.size();
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
		if (startPos != findPos)
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

std::string convertIntToString(int const & n)
{
	std::stringstream	ss;
	std::string			result;

	ss << n;
	ss >> result;
	return result;
}

void	addToMapString(stringMap & map, std::string const & first, std::string const & second)
{
	stringPair	pair(first, second);

	map.insert(pair);
}

void	replaceString(std::string & text, std::string const & replaceText, std::string const & findText)
{
	size_t	pos;

	pos = text.find(findText);
	text.erase(pos, findText.size());
	text.insert(pos, replaceText);
}

std::string	convertStringMapToString(stringMap const & map, std::string const & sep)
{
	std::string	result;

	for (stringMapIt it = map.begin(); it != map.end(); it++)
	{
		result += (*it).first + ": " + (*it).second + sep;
	}
	return result;
}