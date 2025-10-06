/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyLoc.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:59:34 by barandri          #+#    #+#             */
/*   Updated: 2025/09/15 11:45:54 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MyLoc.hpp"
#include "ALocation.hpp"

MyLoc::MyLoc()
{
    _autoindex.push_back("false");
}

MyLoc::MyLoc(MyLoc const &other) : ALocation(other){}

MyLoc &MyLoc::operator=(MyLoc const &other)
{
    if (this != & other)
	{
		ALocation::operator=(other);
	}
	return *this;
}

MyLoc::~MyLoc(){}

void MyLoc::parseLocation(std::string const & line)
{
    stringVect  attr;
    stringVect  elt;
    std::string topic;

    attr = splitString(line, " | ");
    for(stringVectIt it = attr.begin(); it != attr.end(); it++)
    {
        elt = splitString(*it, " ");
        topic = elt[0];
        elt.erase(elt.begin());
        if (topic == "location")
            _location = elt;
        if (topic == "root")
            _root = elt;
        if (topic == "autoindex")
            _autoindex = elt;
        if (topic == "method")
            _method = elt;
        if (topic == "redirect")
            _redirect = elt;
        if (topic == "index")
            _index = elt;
    }
}

