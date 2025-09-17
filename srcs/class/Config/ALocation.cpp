/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ALocation.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:57:54 by barandri          #+#    #+#             */
/*   Updated: 2025/09/17 07:16:00 by trahanta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ALocation.hpp"

std::string ALocation::getAutoindex() const
{
    return _autoindex[0];
}

std::string ALocation::getLocation() const
{
    return _location[0];
}

std::string ALocation::getRootDirectory() const
{
    return _root[0];
}

stringVect ALocation::getAvailableMethod() const
{
    return _method;
}

stringVect ALocation::getRedirect() const
{
    return _redirect;
}

stringVect ALocation::getIndex() const
{
    return _index;
}

void ALocation::displayLocation() const
{
    displayStringVect(_location, "location ");
    std::cout << "{ \n";
    displayStringVect(_root, "  root : ");
    if (_method.size())
        displayStringVect(_method, "    method : ");
    if (_redirect.size())
        displayStringVect(_redirect, "  redirect : ");
    if (_index.size())
        displayStringVect(_index, " index : ");
    std::cout << "} \n";
}

ALocation::~ALocation(){}
