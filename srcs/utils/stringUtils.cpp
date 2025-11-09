/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stringUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:27 by barandri          #+#    #+#             */
/*   Updated: 2025/11/09 20:14:47 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "general.hpp"

stringVect	strictSplit(std::string const & target, std::string const & sep)
{
	size_t		pos;
	stringVect	result;

	pos = target.find_first_of(sep);
	if (pos == std::string::npos)
		throw std::runtime_error("Strict split error");
	result.push_back(target.substr(0, pos));
	pos += sep.size();
	result.push_back(target.substr(pos, target.size() - pos));
    return result;
}

stringVect	splitString(std::string const & target, std::string const & sep)
{
	size_t		findPos;
	size_t		startPos;
	size_t		subSize;
	stringVect	result;

	startPos = 0;
	findPos = 0;
	while (findPos != std::string::npos)
	{
		subSize = 0;
		findPos = target.find(sep, startPos);
		if (findPos == std::string::npos)
			subSize = target.size() - startPos;
		else if (startPos != findPos)
			subSize = findPos - startPos;
		if (subSize)
			result.push_back(target.substr(startPos, subSize));
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

void	addToMapString(stringMap & map, std::string const & first, std::string const & second)
{
	stringPair	pair(first, second);
	map.insert(pair);
}

void	replaceString(std::string & text, std::string const & replaceText, std::string const & findText)
{
	size_t	pos = text.find(findText);
	text.erase(pos, findText.size());
	text.insert(pos, replaceText);
}

char**	convertStringVectToTable(stringVect & vect)
{
	size_t		i;
	size_t		tmpLen;
	std::string	tmp;
	char		*cpyTmp;
	char		**res;

	res = new char*[vect.size() + 1];
	for (i = 0; i < vect.size(); i++)
	{
		tmp = vect[i];
		tmpLen = tmp.size() + 1;
		cpyTmp = new char[tmpLen];
		std::memcpy(cpyTmp, tmp.c_str(), tmpLen);
		res[i] = cpyTmp;
	}
	res[i] = NULL;
	return res;
}

void	clearCharTable(char ***tab)
{
	char **tmp = *tab;

	if (!*tab) 
        return;
    for (int i = 0; tmp[i] != NULL; i++) 
        delete [] tmp[i];
    delete [] *tab;
	*tab = NULL;
}

std::string	divideStringBySize(std::string const & target, size_t const & startPos, size_t const & bufferSize)
{
	size_t		endPos = startPos + bufferSize;
	std::string	result = result = target.substr(startPos, endPos - startPos);;
	return result;
}

intVect containsExactWordInVector(stringVect const & lines, const std::string & word)
{
	intVect index;
	int i = 0;
	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it, i++)
	{
		const std::string &line = *it;

		std::istringstream iss(line);
		std::string w;
		while (iss >> w)
		{
			if (w == word)
				index.push_back(i);
		}
	}
	return (index);
}

bool checkDuplicate(stringVect const & file, std::string const & name, size_t & start, size_t & end)
{
	int count = 0;

	for (size_t i = start; i < end; ++i)
	{
		std::istringstream iss(file[i]);
		std::string word;
		while (iss >> word)
		{
			if (name == word)
				count++;
		}
	}
	if (count != 1)
		return (true);
	return (false);
}

/* [!] : Remove if not necessary at the end*/
stringMapIt findAndGetFromMap(stringMap &map, std::string const &key)
{
	stringMapIt	it;

	for (it = map.begin(); it != map.end(); it++)
	{
		if ((*it).first == key)
			break;
	}
    return it;
}
