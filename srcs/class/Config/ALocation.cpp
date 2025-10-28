/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ALocation.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:57:54 by barandri          #+#    #+#             */
/*   Updated: 2025/10/27 08:28:18 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ALocation.hpp"

ALocation::ALocation()
{
	_autoindex.push_back("OFF");
	_cgiExtension.push_back("OFF");
}

ALocation::ALocation(ALocation const &other) :
		_autoindex(other._autoindex),
		_location(other._location),
		_locations(other._locations),
		_root(other._root),
		_method(other._method),
		_redirect(other._redirect),
		_index(other._index),
		_cgiExtension(other._cgiExtension),
		_checkStats(other._checkStats),
		_redirection(other._redirection){}

ALocation &ALocation::operator=(ALocation const &other)
{
	if (this != &other)
	{
		_autoindex = other._autoindex;
		_location = other._location;
		_locations = other._locations;
		_root = other._root;
		_method = other._method;
		_redirect = other._redirect;
		_index = other._index;
		_checkStats = other._checkStats;
		_cgiExtension = other._cgiExtension;
		_redirection = other._redirection;
	}
	return *this;
}

std::string ALocation::getAutoindex() const
{
	return _autoindex[0];
}

std::string ALocation::getLocation() const
{
	return _locations;
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

std::string ALocation::getCgiActivation() const
{
	return _cgiExtension[0];
}

std::string ALocation::getCgiExecPath() const
{
	return _cgiExtension[1];
}

void ALocation::displayLocation() const
{
	std::cout << "location " << _locations << " {\n";
	displayStringVect(_root, "	root : ");
	if (_method.size())
		displayStringVect(_method, "	method : ");
	if (_redirect.size())
		displayStringVect(_redirect, "	redirect : ");
	if (_index.size())
		displayStringVect(_index, "	index : ");
	if (_autoindex.size())
		displayStringVect(_autoindex, "	autoindex : ");
	if (_cgiExtension.size())
		displayStringVect(_cgiExtension, "	cgi : ");
	std::cout << "} \n";
}
