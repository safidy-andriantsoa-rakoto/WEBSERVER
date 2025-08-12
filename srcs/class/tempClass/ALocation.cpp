/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ALocation.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:57:54 by barandri          #+#    #+#             */
/*   Updated: 2025/08/12 14:09:54 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ALocation.hpp"

stringVect ALocation::getAutoindex() const
{
    return _autoindex;
}

stringVect ALocation::getLocation() const
{
    return _location;
}

stringVect ALocation::getRootDirectory() const
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
    displayStringVect(_location, "location ");
    std::cout << "{ \n";
    displayStringVect(_root, "  root : ");
    if (_method.size() > 1)
        displayStringVect(_method, "    method : ");
    if (_redirect.size() > 1)
        displayStringVect(_redirect, "  redirect : ");
    if (_index.size() > 1)
        displayStringVect(_index, " index : ");
    std::cout << "} \n";
}
