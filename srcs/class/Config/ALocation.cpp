/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ALocation.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:57:54 by barandri          #+#    #+#             */
/*   Updated: 2025/09/15 11:44:40 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ALocation.hpp"

ALocation::ALocation(){}

ALocation::ALocation(ALocation const &other) :
        _autoindex(other._autoindex),
        _location(other._location),
        _root(other._root),
        _method(other._method),
        _redirect(other._redirect),
        _index(other._index){}

ALocation &ALocation::operator=(ALocation const &other)
{
    if (this != &other)
    {
        _autoindex = other._autoindex;
        _location = other._location;
        _root = other._root;
        _method = other._method;
        _redirect = other._redirect;
        _index = other._index;
    }
    return *this;
}

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
