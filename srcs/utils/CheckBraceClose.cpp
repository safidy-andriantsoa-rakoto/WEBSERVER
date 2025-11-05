/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckBraceClose.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 10:13:07 by trahanta          #+#    #+#             */
/*   Updated: 2025/11/05 15:48:01 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "general.hpp"

bool    checkIfContentInsideBracket(std::string const & openString, int const & open, int const & close)
{
	std::cout << "String = [" << openString << "]" << std::endl;
	std::cout << "Open = " << openString[open] << "; Close = " << openString[close] << std::endl;
	std::cout << "Open = " << open << "; Close = " << close << std::endl;
	for(int i = open + 1; i <= close; i++)
	{
		if (std::isspace(openString[i]) == true)
			return false;
		std::cout << "[" << openString[i] << "]" << std::endl;
	}
	return true;
}

void	checkBracketIndex(stringVect const & vect, intPairVect & bracketIndex)
{
	int		openB;
	int		closeB;
	int		vectIndex;
	size_t	i;
	size_t	j;
	size_t	size = bracketIndex.size();
	std::string	sub;

	std::cout << "Size = " << size << " " << size / 2 << std::endl;
	for (i = 0; i < size / 2; i++)
	{
		j = size - i - 1;
		vectIndex = bracketIndex[i].first;
		openB = bracketIndex[i].second;
		closeB = bracketIndex[j].second;
		if (checkIfContentInsideBracket(vect[vectIndex], openB, closeB))
			throw std::runtime_error("Error : There is nothing inside a bracket");
	}
}

bool	checkBraceClose(std::vector<std::string> vect)
{
	int			count = 0;
	intPairVect	index;

	for (size_t i = 0; i < vect.size(); i++)
	{
		std::cout << "[" << vect[i] << "]" << std::endl;
		for (size_t j = 0; j < vect[i].size(); j++)
			if (vect[i][j] == '{')
			{
				index.push_back(std::pair<int, int>(i, j));
				count++;
			}
			else if (vect[i][j] == '}')
			{
				index.push_back(std::pair<int, int>(i, j));
				count--;
				if (count < 0)
				{
					std::cerr << "Error : brace not close in config" << std::endl;/* [UPDATE] : cout to cerr */
					return false;
				}
			}
	}
	if (count > 0)
	{
		std::cerr << "Error : brace not close in config" << std::endl;/* [UPDATE] : cout to cerr */
		return false;
	}
	checkBracketIndex(vect, index);
	std::cout << "CHECK BRACE CLOSE DONE !!!!!" << std::endl;
	return true;
}

bool checkBracesAfterNewline(const std::vector<std::string> &file)
{
	for (size_t i = 0; i < file.size(); ++i)
	{
		const std::string &line = file[i];

		for (size_t j = 0; j < line.size(); ++j)
		{
			char c = line[j];

			if (c == '{' || c == '}')
			{
				if (j + 1 < line.size())
				{
					char nextChar = line[j + 1];

					if (nextChar != '\n' && nextChar != ' ')
					{
						std::cerr << "Error: Character after '"
								  << c << "' is not allowed '" << std::endl;
						return false; /* [UPDATE] : add return */
					}
				}
				else
				{
					continue;
				}
			}
		}
	}
	return true;
}
