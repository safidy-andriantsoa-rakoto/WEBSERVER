/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyConf.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:59:22 by barandri          #+#    #+#             */
/*   Updated: 2025/11/05 11:40:28 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MyConf.hpp"
#include "AConfig.hpp"

MyConf::MyConf()
{
	_hasErrPage = false;
}

MyConf::MyConf(MyConf const & other) : AConfig(other){}

MyConf &MyConf::operator=(MyConf const &other)
{
    if (this != & other)
	{
		AConfig::operator=(other);
	}
	return *this;
}

MyConf::~MyConf(){}

void MyConf::init_port_child(int port)
{
	if (_port.size())
		_port.clear();
	_port.push_back(port);
}

void MyConf::delete_port_child_in_parent()
{
	if (_port.size() > 1)
	{
		int port = _port[0];
		_port.clear();
		_port.push_back(port);	
	}
}

void	MyConf::displayConfig() const
{
	std::cout << "------------------------------------------" << std::endl;
	std::cout << "Host :" << _host << std::endl;
	std::cout << "Port :";
	for (intVectIt itInt = _port.begin(); itInt != _port.end(); itInt++)
		std::cout << " " << (*itInt);
	std::cout << std::endl;
	if (_errPage.listError.size())
	{
		std::cout << "error_page: ";
		displayStringVect(_errPage.listError, "List error");
		std::cout << "Error file = " << _errPage.filePath << std::endl;
	}
	for (vectLocationcIt it = _location.begin(); it != _location.end(); it++)
	{
		(*it).displayLocation();
		std::cout << std::endl;
	}
}

void MyConf::getElementConfig(const size_t &start, const size_t &end, const stringVect &file)
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
				{
					for (size_t i = 0; i < word.size(); i++)
					if (!isdigit(word[i]))
						throw std::runtime_error ("Error : Find not digit element");
					_port.push_back(atoi(word.c_str()));
				}
				if (name == "size")
					_size.push_back(word);
				if (name == "error_page")
					_errPage.strError.push_back(word);
			}
		}
		if (name == "location")
			countLoc++;
	}
	if(checkValuePort(_port) == false)
		throw std::runtime_error ("Error : Port invalid");
	if(checkDuplicationStr(_errPage.strError) == false)
		throw std::runtime_error ("Error : server name invalid");
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
	getElementConfig(start, end, file);
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
		{
			_checkStats = false;
			return;
		}
	}
	_checkStats = true;
	currentBlock++;
}

vectLocation MyConf::createVectLocation(size_t countLocation)
{
    vectLocation tabLocation;
    MyLoc  loc;
      	for (size_t i = 0; i < countLocation; ++i)
        {
            loc = MyLoc();
            tabLocation.push_back(loc);
        }
    return (tabLocation);
}

void MyConf::setHostName(std::string host)
{
	if (host.size())
		this->_host = host;
}
