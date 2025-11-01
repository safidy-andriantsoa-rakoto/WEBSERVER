/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversion.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 08:02:39 by barandri          #+#    #+#             */
/*   Updated: 2025/10/31 07:27:34 by barandri         ###   ########.fr       */
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
		result += (*it).first + ": " + (*it).second + sep;
	return result;
}

float	convertToBytes(std::string const & n)
{
	int					exp;
	float				value;
	std::string			unit;
	std::stringstream	ss;

	ss << n;
	ss >> value;
	ss >> unit;
	exp = 0;
	if (unit == "KB")
		exp = 3;
	else if (unit == "MB")
		exp = 6;
	else if (unit == "GB")
		exp = 9;
	value *= std::pow(10, exp);
	return (value);
}