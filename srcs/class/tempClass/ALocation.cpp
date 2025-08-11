/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ALocation.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:57:54 by barandri          #+#    #+#             */
/*   Updated: 2025/08/11 20:35:35 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ALocation.hpp"

bool ALocation::getAutoindex() const
{
    return _autoindex;
}

std::string ALocation::getLocation() const
{
    return _location;
}

std::string ALocation::getRootDirectory() const
{
    return _root;
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
    std::cout << "Location " << _location << " {\n"
    << "    Root " << _root << std::endl;
    if (_method.size())
        displayStringVect(_method, "method");
    if (_redirect.size())
        displayStringVect(_redirect, "redirect");
    if (_index.size())
        displayStringVect(_index, "index");
    std::cout << "} \n";
}
