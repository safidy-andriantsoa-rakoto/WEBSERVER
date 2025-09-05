/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversion.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 08:02:39 by barandri          #+#    #+#             */
/*   Updated: 2025/09/03 18:12:58 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "general.hpp"

std::string convertIntToString(ssize_t const & n)
{
	std::stringstream	ss;
	std::string			result;

	ss << n;
	ss >> result;
	return result;
}

float convertStringToFLoat(std::string const & n)
{
	std::stringstream	ss;
	float				result;

	ss << n;
	ss >> result;
	return result;
}

int convertStringToInt(std::string const & n)
{
	std::stringstream	ss;
	int					result;

	ss << n;
	ss >> result;
	return result;	
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