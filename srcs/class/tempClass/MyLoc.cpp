/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyLoc.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:59:34 by barandri          #+#    #+#             */
/*   Updated: 2025/08/12 13:55:46 by barandri         ###   ########.fr       */
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
    for(stringVectIt it = attr.begin(); it != attr.end(); it++)
    {
        elt = splitString(*it, " ");
        if (elt[0] == "location")
            _location = elt;
        else if (elt[0] == "root")
            _root = elt;
        else if (elt[0] == "autoindex")
            _autoindex = elt;
        else if (elt[0] == "method")
            _method = elt;
        else if (elt[0] == "redirect")
            _redirect = elt;
        else if (elt[0] == "index")
            _index = elt;
    }
}

