/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stringUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:27 by barandri          #+#    #+#             */
/*   Updated: 2025/09/30 14:18:01 by barandri         ###   ########.fr       */
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

void	chunkingString(std::string & text, int const & status)
{
	std::string			add;
	std::stringstream	ss;
	size_t				size = text.size();

	ss << std::hex << size;
	ss >> add;
	text.insert(0, add + "\r\n");
	if (status == OP_CLOSE)
		text += "\r\n0\r\n";
}

char**	convertStringVectToTable(stringVect & vect)
{
	char	**res;
	size_t	size = vect.size() + 1;
	size_t	sizeIntern;
	size_t	i;

	std::cout << "CONVERT STRINGVECT TO TABLE : " << size << std::endl;
	if (!vect.size())
		return NULL;
	res = static_cast<char **>(calloc(size, sizeof(char *)));
	if (!res)
		return NULL;
	i = 0;
	for (stringVectIt it = vect.begin(); it != vect.end(); it++)
	{
		sizeIntern = (*it).size() + 1;
		res[i] = static_cast<char *>(calloc(sizeIntern, sizeof(char)));
		if (!res[i])
		{
			clearCharTable(res);
			return NULL;
		}
		strcpy(res[i], (*it).c_str());
		i++;
	}
	res[i] = NULL;
	return res;
}

void	clearCharTable(char **tab)
{
	char	*tmp;
	char	**tmpAll = tab;

	if (!tab)
		return;
	while (*tab)
	{
		tmp = *tab;
		tab++;
		free(tmp);
	}
	free(tmpAll);
}
