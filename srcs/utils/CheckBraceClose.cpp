/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckBraceClose.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 10:13:07 by trahanta          #+#    #+#             */
/*   Updated: 2025/11/05 22:11:30 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "general.hpp"

bool    checkStringInsideBrace(std::string const & content, int const & open, int const & close)
{
	int	count = 0;

	for(int i = open + 1; i <= close; i++)
	{
		if (std::isspace(content[i]) == false && content[i] != '\n')
			return true;
		count++;
	}
	if (count)
		return true;
	std::cout << "COUNT = " <<  count << std::endl;
	return false;
}

void	checkBraceContent(stringVect const & vect, intPairVect const & braceIndex)
{
	int		openB;
	int		closeB;
	size_t	vect_i;
	size_t	vect_j;
	size_t	size = braceIndex.size();
	size_t	j;

	// std::cout << "Size = " << size << " " << size / 2 << std::endl;
	for(size_t i = 0; i < size; i++)
	{
		j = size - i - 1;
		vect_i = braceIndex[i].first;
		vect_j = braceIndex[j].first;
		for (; vect_i < vect_j; vect_i++)
		{
			openB = braceIndex[i].second;
			closeB = braceIndex[j].second;
			if (checkStringInsideBrace(vect[vect_i], openB, closeB) == false)
				throw std::runtime_error("Error : There is nothing inside a bracket");
		}
	}
}

/* [UPDATE] : Review argument function */
bool	checkBraceClose(stringVect const & vect, intPairVect & index)
{
	int	count = 0;

	for (size_t i = 0; i < vect.size(); i++)
	{
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
	return true;
}

/* [UPDATE] : Add checkBraceContent */
bool checkBracesAfterNewline(stringVect const & file, intPairVect const & braceIndex)
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
		(void) braceIndex;
		// checkBraceContent(file, braceIndex);
	}
	return true;
}
