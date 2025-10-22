/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfig.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:57:41 by barandri          #+#    #+#             */
/*   Updated: 2025/10/22 11:44:23 by trahanta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AConfig.hpp"

AConfig::AConfig(){}

AConfig::AConfig(AConfig const &other) : 
	_port(other._port),
	_ports(other._ports),
	_maxSize(other._maxSize),
	_host(other._host),
	_serverName(other._serverName),
	_errPage(other._errPage),
	_hasErrPage(other._hasErrPage),
	_location(other._location),
	_size(other._size){}

AConfig &AConfig::operator=(AConfig const &other)
{
    if (this != &other)
    {
        _port       = other._port;
		_ports  = other._ports;
        _maxSize    = other._maxSize;
        _host       = other._host;
        _serverName = other._serverName;
        _errPage    = other._errPage;
        _hasErrPage = other._hasErrPage;
        _location   = other._location;
		_size = other._size;
    }
    return *this;
}

int AConfig::getServPort() const
{
	return _port;
}

intVect AConfig::getServerPort() const
{
	return _ports;
}


float	AConfig::getMaxSize() const
{
	return _maxSize;
}

std::string	AConfig::getServHost() const
{
	return _host;
}

stringVect	AConfig::getServName() const
{
	return _serverName;
}

t_errPage	AConfig::getErrPage() const
{
	return _errPage;
}

vectLocation AConfig::getClassLocation() const
{
	return _location;
}

vectLocationIt AConfig::findClassLocation(std::string const & toFind)
{
	vectLocationIt	it;

	for (it = _location.begin(); it != _location.end(); it++)
	{
		std::cout << (*it).getLocation() << std::endl;
		if ((*it).getLocation() == toFind)
			break ;
	}
    return it;
}
