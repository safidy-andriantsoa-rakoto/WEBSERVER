/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyConf.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:59:22 by barandri          #+#    #+#             */
/*   Updated: 2025/09/10 13:27:23 by trahanta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MyConf.hpp"
#include "AConfig.hpp"

MyConf::MyConf()
{
	_hasErrPage = false;
}

MyConf::MyConf(MyConf const &other)
{
	*this = other;
}

MyConf &MyConf::operator=(MyConf const &other)
{
	if (this != &other)
	{
		_port = other._port;
		_host = other._host;
		_serverName = other._serverName;
		_errPage = other._errPage;
		_hasErrPage = other._hasErrPage;
		_location = other._location;
	}
	return *this;
}

MyConf::~MyConf() {}

void MyConf::parseConfig(std::string const &filePath)
{
	std::ifstream file;
	std::string line;
	MyLoc loc;

	file.open(filePath.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error on parsing config");
	while (std::getline(file, line))
	{
		loc = MyLoc();
		loc.parseLocation(line);
		_location.push_back(loc);
	}
	loc.parseLocation("location /errorPage | root ./errorPage/default | method GET POST DELETE"); // Add default error page location
	_location.push_back(loc);
}

bool MyConf::getSatuts()
{
	return (_checkStats);
}

// void MyConf::parseConfig(std::string const &servPath, std::string const &locPath)
// {
// 	std::ifstream	file;
// 	std::string		line;
// 	stringVect		vect;

// 	file.open(servPath.c_str());
// 	if (!file.is_open())
// 		throw std::runtime_error("Error on parsing config 1");
// 	while (std::getline(file, line))
// 	{
// 		vect = splitString(line, " ");
// 		if (vect[0] == "host")
// 			_host = vect[1];
// 		else if (vect[0] == "port")
// 			_port = convertStringToInt(vect[1]);
// 		else if (vect[0] == "server_names")
// 		{
// 			vect.erase(vect.begin());
// 			_serverName = vect;
// 		}
// 		else if (vect[0] == "error_page")
// 		{
// 			_hasErrPage = true;
// 			_errPage.filePath = *(vect.end() - 1);
// 			vect.erase((vect.end() - 1));
// 			vect.erase(vect.begin());
// 			_errPage.listError = vect;
// 			_errPage.fileContent = readFileToString(_errPage.filePath);
// 		}
// 		else if (vect[0] == "size")
// 			_maxSize = convertStringToFLoat(vect[1]);
// 		vect.clear();
// 		line.clear();
// 	}
// 	parseConfig(locPath);
// }

// void MyConf::parseConfig(stringVect file)
// {
// 	intVect index = containsExactWordInVector(file, "server");
// 	int test(0), j(1), k(0);
// 	for (size_t i = 0; i < file.size(); i++)
// 	{
// 		std::string word;
// 		_checkStats = false;
// 		while (j < index.size())
// 		{
// 			while (k != index[j + 1])
// 			{
// 				std::istringstream iss(file[j]);
// 				if (iss >> word)
// 				{
// 					if (word == "host")
// 						_host = word;
// 				}

// 				_checkStats = true;
// 			}
// 			j = index[j + 1];
// 		}
// 	}
// }

void MyConf::displayConfig() const
{
	std::cout << "------------------------------------------" << std::endl;
	std::cout << "Host " << _host << std::endl
			  << "Port " << _port << std::endl;
	if (_serverName.size())
		displayStringVect(_serverName, "Server name");
	if (_errPage.listError.size())
	{
		displayStringVect(_errPage.listError, "List error");
		std::cout << "Error file = " << _errPage.filePath << std::endl;
	}
	for (vectLocationcIt it = _location.begin(); it != _location.end(); it++)
	{
		(*it).displayLocation();
		std::cout << std::endl;
	}
}

void MyConf::parseConfig(stringVect file)
{
	static size_t currentBlock = 0;
	static intVect index = containsExactWordInVector(file, "server");

	if (currentBlock >= index.size())
		return;

	size_t start = index[currentBlock];
	size_t end;
	if (currentBlock + 1 < index.size())
		end = index[currentBlock + 1];
	else
		end = file.size();
	for (size_t i = start; i < end; ++i)
	{
		std::istringstream iss(file[i]);
		std::string key, value;
		iss >> key >> value;
	}

	_checkStats = true;
	currentBlock++;
}