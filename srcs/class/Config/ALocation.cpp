/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ALocation.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:57:54 by barandri          #+#    #+#             */
/*   Updated: 2025/11/17 15:05:44 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ALocation.hpp"

ALocation::ALocation()
{
	_autoindex.push_back("OFF");
	_cgiExtension.push_back("OFF");
	_index.push_back("index.html");
}

ALocation::ALocation(std::string const &locationName) : _name(locationName)
{
	_autoindex.push_back("OFF");
	_cgiExtension.push_back("OFF");
	_index.push_back("index.html");
}

ALocation::ALocation(ALocation const &other) :
		_autoindex(other._autoindex),
		_name(other._name),
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
		_name = other._name;
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

std::string ALocation::getName() const
{
	return _name;
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

std::string ALocation::getIndex() const
{
	return _index[0];
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
	std::cout << "location " << _name  << std::endl;
	displayStringVect(_root, "	root ");
	if (_method.size())
		displayStringVect(_method, "	method ");
	if (_redirect.size())
		displayStringVect(_redirect, "	redirect ");
	if (_index.size())
		displayStringVect(_index, "	index ");
	if (_autoindex.size())
		displayStringVect(_autoindex, "	autoindex ");
	if (_cgiExtension.size())
		displayStringVect(_cgiExtension, "	cgi ");
}
