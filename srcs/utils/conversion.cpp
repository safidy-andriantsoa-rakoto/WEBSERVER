/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversion.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 08:02:39 by barandri          #+#    #+#             */
/*   Updated: 2025/11/18 13:34:27 by rsafidy-         ###   ########.fr       */
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
	if (ss.eof() == false)
		throw std::runtime_error("SConversion from string to float failed");
	return result;
}

int convertStringToInt(std::string const & n)
{
	std::stringstream	ss;
	int					result;

	ss << n;
	ss >> result;
	if (ss.eof() == false)
		throw std::runtime_error("Conversion from string to int failed");
	return result;	
}

std::string	convertStringMapToString(stringMap const & map, std::string const & sep)
{
	std::string	result;

	for (stringMapIt it = map.begin(); it != map.end(); it++)
		result += (*it).first + ": " + (*it).second + sep;
	return result;
}

float	convertToBytes(stringVect const & vect)
{
	int					exp;
	float				value;
	std::string			unit;
	std::stringstream	ss;

	ss << vect[0];
	ss >> value;
	if (ss.eof() == false || ss.fail() == true)
		throw std::runtime_error("Value is not valid");
	unit = vect[1];
	exp = 0;
	if (unit == "MB")
		exp = 6;
	else if (unit == "GB")
		exp = 9;
	else
		throw std::runtime_error("Conversion failed");
	value *= std::pow(10, exp);
	value += 0.1;
	return (value);
}