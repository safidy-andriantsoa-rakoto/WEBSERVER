/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyConf.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:59:22 by barandri          #+#    #+#             */
/*   Updated: 2025/10/20 14:34:01 by trahanta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MyConf.hpp"
#include "AConfig.hpp"

MyConf::MyConf()
{
	_hasErrPage = false;
}

MyConf::MyConf(MyConf const &other) : AConfig(other) {}

MyConf &MyConf::operator=(MyConf const &other)
{
	if (this != &other)
	{
		AConfig::operator=(other);
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
		loc.myParseLocation(line);
		_location.push_back(loc);
	}
	vectLocationIt it = findClassLocation("upload");
	if (it == _location.end())
	{
		loc = MyLoc();
		loc.myParseLocation(DEF_UPLOAD_LOCATION); // Add default upload location
		_location.push_back(loc);
	}
}

void MyConf::parseConfig(std::string const &servPath, std::string const &locPath)
{
	std::ifstream file;
	std::string line;
	stringVect vect;

	file.open(servPath.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error on parsing config 1");
	while (std::getline(file, line))
	{
		vect = splitString(line, " ");
		if (vect[0] == "host")
			_host = vect[1];
		else if (vect[0] == "port")
			_port = convertStringToInt(vect[1]);
		else if (vect[0] == "server_names")
		{
			vect.erase(vect.begin());
			_serverName = vect;
		}
		else if (vect[0] == "error_page")
		{
			_hasErrPage = true;
			_errPage.filePath = *(vect.end() - 1);
			vect.erase((vect.end() - 1));
			vect.erase(vect.begin());
			_errPage.listError = vect;
			_errPage.fileContent = readFileToString(_errPage.filePath);
		}
		else if (vect[0] == "size")
			_maxSize = convertStringToFLoat(vect[1]);
		vect.clear();
		line.clear();
	}
	parseConfig(locPath);
}

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

void MyConf::getElemetConfig(const size_t &start, const size_t &end, const stringVect &file)
{
	int countLoc = 0;
	_countLocation = 0;
	for (size_t i = start; i < end; i++)
	{
		std::istringstream iss(file[i]);
		std::string name, value;
		iss >> name;
		while (std::getline(iss, value))
		{
			std::string word;
			std::istringstream s(value);
			while (s >> word)
			{
				if (name == "host")
					_host = word;
				if (name == "port")
					_port = atoi(word.c_str());
				if (name == "server_names")
					_serverName.push_back(word);
				if (name == "size")
					_size.push_back(word);
				if (name == "error_page")
					_errPage.strError.push_back(word);
			}
		}
		if (name == "location")
			countLoc++;
	}
	_countLocation = countLoc;
}

bool MyConf::getStatus()
{
	return _checkStats;
}

size_t MyConf::findNotDigitError()
{
	size_t checkIndex(0), i(0);
	while (i < _errPage.strError.size())
	{
		std::string str = _errPage.strError[i];
		bool isAllDigit = true;
		for (size_t j = 0; j < str.size(); j++)
		{
			if (!isdigit(str[j]))
			{
				isAllDigit = false;
				break;
			}
		}
		if (isAllDigit && !str.empty())
		{
			_errPage.listError.push_back(str);
		}
		else if (!isAllDigit)
		{
			checkIndex = i;
			break;
		}
		i++;
	}
	return (checkIndex);
}
vectLocation MyConf::createVectLocation(size_t countLocation)
{
	vectLocation tabLocation;
	MyLoc loc;
	for (size_t i = 0; i < countLocation; ++i)
	{
		loc = MyLoc();
		tabLocation.push_back(loc);
	}
	return (tabLocation);
}
void MyConf::parseConfig(stringVect file)
{
	static size_t currentBlock(0);
	size_t i(0), k(0);
	static intVect index = containsExactWordInVector(file, "server");
	if (currentBlock >= index.size())
		return;
	size_t start = index[currentBlock];
	size_t end;
	if (currentBlock + 1 < index.size())
		end = index[currentBlock + 1];
	else
		end = file.size();
	_endLocation = end;
	if (checkConfigDupli(file, start, end))
	{
		_checkStats = false;
		return;
	}
	getElemetConfig(start, end, file);
	i = findNotDigitError();
	if (i > 0 && ((_errPage.strError.size() - i > 1)))
	{
		_checkStats = false;
		return;
	}
	else if (i > 0 && i < _errPage.strError.size())
		_errPage.filePath = _errPage.strError[i];
	this->_location = createVectLocation(_countLocation);
	while (k < _location.size())
	{
		_location[k].parseLocation(file);
		if (_location[k].getStatus())
			k++;
		else
			return;
	}
	_checkStats = true;
	currentBlock++;
}
