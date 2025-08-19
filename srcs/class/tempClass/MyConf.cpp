/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyConf.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:59:22 by barandri          #+#    #+#             */
/*   Updated: 2025/08/19 10:49:44 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MyConf.hpp"
#include "AConfig.hpp"

MyConf::MyConf(){}

MyConf::MyConf(int const &port){	
	_host = "localhost";
	_port = port;
	_maxSize = 50;
}

MyConf::MyConf(MyConf const & other)
{
	*this = other;
}

MyConf &MyConf::operator=(MyConf const &other)
{
    if (this != & other)
	{
		_port = other._port;
		_host = other._host;
		_serverName = other._serverName;
		_errPage = other._errPage;
		_location = other._location;
	}
	return *this;
}

MyConf::~MyConf(){}

void MyConf::parseConfig(std::string const & filePath)
{
	std::ifstream	file;
	std::string		line;
	MyLoc			loc;

	file.open(filePath.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error on parsing config");
	while (std::getline(file, line))
	{
		loc = MyLoc();
		loc.parseLocation(line);
		_location.push_back(loc);
	}
}

void	MyConf::displayConfig() const
{
	std::cout << "------------------------------------------" << std::endl;
	std::cout << "Host " << _host << std::endl
	<< "Port " << _port << std::endl;
	if (_serverName.size())
		displayStringVect(_serverName, "Server name");
	if (_errPage.listError.size())
	{
		displayStringVect(_errPage.listError, "List error");
		std::cout << "Error file = " << _errPage.fileName << std::endl;
	}
	for (vectLocationcIt it = _location.begin(); it != _location.end(); it++)
	{
		(*it).displayLocation();
		std::cout << std::endl;
	}
}


