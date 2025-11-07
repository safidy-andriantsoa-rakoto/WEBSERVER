/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 09:43:53 by barandri          #+#    #+#             */
/*   Updated: 2025/11/07 19:14:07 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "general.hpp"

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

void	displayString(std::string const & text)
{
	std::cout << "[" << text << "] ";
}

void	displayStringVect(stringVect const & vect, std::string const & title)
{
	std::cout << title << " ";
	std::for_each(vect.begin(), vect.end(), displayString);
	std::cout << std::endl;
}

void	displayStringVect(stringVect const & vect)
{
	std::for_each(vect.begin(), vect.end(), displayString);
	std::cout << std::endl;
}

void	displayCharTable(char	**tab, std::string const & title)
{
	char	**tmp = tab;

	if (!tmp)
	return;
	std::cout << "=======================" << title << "=======================" << std::endl;
	while (*tmp)
	{
		std::cout << *tmp << std::endl;
		tmp++;
	}
	std::cout << "===========End of " << title << "==============" << std::endl << std::endl;
}

void	displayServerVectConfig(serverVect & allServer_conf)
{
	for (size_t i = 0; i < allServer_conf.size(); i++)
	{
		std::cout << "**********************************************" << std::endl;
		for (size_t j = 0; j < allServer_conf[i].size(); j++)
		{
			if (allServer_conf[i][j].lvl == 2)
				std::cout << "	";
			std::cout << "LVL=" << allServer_conf[i][j].lvl;
			std::cout << "| key=" << allServer_conf[i][j].key;
			displayStringVect(allServer_conf[i][j].value, "	| value=");
		}
	}	
}

void print_conf(serverVect & allServer_conf)
{
	for (size_t i = 0; i < allServer_conf.size(); i++)
	{
		std::cout << "==================================="<< i << "===================================" << std::endl;
		for (size_t j = 0; j < allServer_conf[i].size(); j++)
		{
			std::cout << "LVL=" << allServer_conf[i][j].lvl << std::endl;
			std::cout << "key=" << allServer_conf[i][j].key << std::endl;
			for (size_t k = 0; k < allServer_conf[i][j].value.size(); k++)
				std::cout << "	value=" << allServer_conf[i][j].value[k] << std::endl;
		}
		std::cout << "==================================="<< i << "===================================" << std::endl;
	}
}

void	displayInt(int const & n)
{
	std::cout << "[" << n << "] ";
}

void	displayIntVect(intVect const & vect, std::string const & title)
{
	std::cout << title << " ";
	std::for_each(vect.begin(), vect.end(), displayInt);
	std::cout << std::endl;
}	
