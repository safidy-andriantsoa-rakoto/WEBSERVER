/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyLoc.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:59:34 by barandri          #+#    #+#             */
/*   Updated: 2025/08/11 21:09:51 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MyLoc.hpp"
#include "ALocation.hpp"

MyLoc::MyLoc(){}

MyLoc::~MyLoc(){}

void MyLoc::parseLocation(std::string const & line)
{
    stringVect  attr;
    stringVect  elt;

    attr = splitString(line, " | ");
    if (attr.size() == 2)
    {
        _location = attr[0];
        _root = attr[1];
        // displayLocation();
        return ; 
    }
    if (attr[AUTOINDEX] == "on")
        _autoindex = true;
    else
        _autoindex = false;
    _method = splitString(attr[METHOD], " ");
    _redirect = splitString(attr[REDIRECT], " ");
    _index = splitString(attr[INDEX], " ");
}

