/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   units.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 07:58:44 by barandri          #+#    #+#             */
/*   Updated: 2025/08/21 08:31:08 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "general.hpp"

//Mety tsy ilaina !!!
std::string	checkLimitSize(std::string const & a, std::string const & b)
{
	float				value[2];
	std::string			unit[2];
	std::stringstream	ss;
	std::string			get;
	std::string			result;

	get = a;
	for(int i = 0; i < 2; i++)
	{
		if (i == 1)
			get = b;
		ss << get;
		ss >> value[i];
		ss >> unit[i];
		ss.clear();
	}
	//Assuming there is only kb kylobytes and mb megabytes
	// std::cout << "a = " << value[0] << " " << unit[0] << " ; b = " << value[1] << " " << unit[1] << std::endl;
	if (unit[0] == "mb")
		value[0] *= 1000;
	if (unit[1] == "mb")
		value[1] *= 1000;
	if (value[0] < value[1])
		result = "<";
	else if (value[0] == value[1])
		result = "==";
	else if (value[0] > value[1])
		result = ">";
	return result;
}

//Ilaine any @ parsingConfig => Resahana Tsiky !!!!
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