/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   displayUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 09:43:53 by barandri          #+#    #+#             */
/*   Updated: 2025/08/11 20:34:32 by barandri         ###   ########.fr       */
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
	std::cout << "[" << text << "]" << std::endl;
}

void	displayStringVect(stringVect const & vect, std::string const & title)
{
	std::cout << title << " : ";
	std::for_each(vect.begin(), vect.end(), displayString);	
}