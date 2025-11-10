/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyConf.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:59:22 by barandri          #+#    #+#             */
/*   Updated: 2025/11/10 18:49:21 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MyConf.hpp"
#include "AConfig.hpp"

void MyConf::checkConfigElement(vectData & data)
{
	stringVect	vectElt;
	stringVect	vectLoc;
	vectDataIt	upperLvl = data.begin();
	bool		hasLocationMandatory = false;

	for (vectDataIt it = data.begin(); it != data.end(); it++)
	{
		if ((*it).lvl == 1)
		{
			if ((*it).key.size() == 0 || (*it).value.size() == 0)
				throw std::runtime_error("Server's element need at least 1 value");
			if ((*it).key == "location")
			{
				if ((*it).value.size() != 1)
					throw std::runtime_error("Location has only one value");
				if ((*it).value[0] == "/")
					hasLocationMandatory = true;
				vectLoc.push_back((*it).value[0]);
			}
			else
				vectElt.push_back((*it).key);
			upperLvl = it;
		}
		else if ((*upperLvl).lvl == 2 || (*upperLvl).key != "location")
			throw std::runtime_error("Only location has level 2 elements");
	}
	if (vectLoc.size() == 0)
		throw std::runtime_error("No location");
	if ((vectLoc.size() == 1 && vectLoc[0] != "/") || hasLocationMandatory == false)
		throw std::runtime_error("Must have location /");
	if (vectElt.size() == 0 || vectElt.size() < 3 || vectElt.size() > 4)
		throw std::runtime_error("Server bloc's element is incorrect");
	if (checkDuplicationStr(vectElt) == false)
		throw std::runtime_error("Duplicate among the server element");
	if (checkDuplicationStr(vectLoc) == false)
		throw std::runtime_error("Duplicate among the location bloc");
	checkMandatoryElement(vectElt);
}

void MyConf::checkMandatoryElement(stringVect &vectElt)
{
	stringVect	mandatory;

	mandatory.push_back("host");
	mandatory.push_back("port");
	mandatory.push_back("size");
	if (findEltInStringVect(vectElt, mandatory) == false)
		throw std::runtime_error("host, port and size are mandatory for config element");
}

void MyConf::manageUploadLocation()
{
	vectLocationIt	itFirst;
	vectLocationIt	itUp;
	std::string		rootUpload;

	itFirst = findClassLocation("/");
	itUp = findClassLocation("/upload");
	if (itUp == _location.end())
	{
		MyLoc loc;
		rootUpload = (*itFirst).getRootDirectory() + "/upload";
		std::string	locationUpload = DEFAULT_UPLOAD_LOCATION;
		replaceString(locationUpload, rootUpload, "$1");
		stringVect vect = splitString(locationUpload, "\n");
		loc.specialParseLocation(vect);
		_location.push_back(loc);
	}
	else
	{
		rootUpload = (*itUp).getRootDirectory();
		if (checkIfParentDirectory((*itFirst).getRootDirectory(), rootUpload) == false)
			throw std::runtime_error("Root for location /upload need to be directly inside root for location /");
		if (checkIfDirectoryAvailable(rootUpload) == false)
        	throw std::runtime_error("Upload directory can't be reached");
	}
}

void MyConf::parseHost(stringVect const & vect)
{
	if (vect.size() != 1)
		throw std::runtime_error("Host contains only one value");
	_host = vect[0];
}

void MyConf::parsePort(stringVect const & vect, intVect & allPort)
{
	int	n;

	if (vect.size() == 0 || vect.size() > 3)
		throw std::runtime_error("The number of ports per server is limited between 1 to 3");
	for (stringVectCIt it = vect.begin(); it != vect.end(); it++)
	{
		try
		{
			n = convertStringToInt(*it);
			allPort.push_back(n);
			_port.push_back(n);
		}
		catch(const std::exception& e)
		{
			throw std::runtime_error("There is an invalid port");
		}
	}
	if (checkValuePort(_port) == false)
		throw std::runtime_error("There is an invalid port");
	if (checkDuplicationInt(_port) == false || checkDuplicationInt(allPort) == false)
		throw std::runtime_error("The port already used by a server");
}

void MyConf::parseErrorPage(stringVect const & vect)
{
	stringVect	allStatusCode = splitString(ALL_STATUS_CODE, ";");
	size_t		i;
	stringVect	listStatusCode;

	if (vect.size() < 2)
		throw std::runtime_error("Error_page's values are missing");
	if (vect[0] == "ALL")
	{
		if (vect.size() != 2)
			throw std::runtime_error("Error_page with option ALL requires only 2 values");
		listStatusCode = allStatusCode;
		i = 1;
	}
	else
	{
		for(i = 0; i < vect.size() - 1; i++)
		{
			if (std::find(allStatusCode.begin(), allStatusCode.end(), vect[i]) == allStatusCode.end())
				throw std::runtime_error("There is an invalid status code");
			listStatusCode.push_back(vect[i]);
			if (listStatusCode.size() > allStatusCode.size())
				throw std::runtime_error("Status code limit exceeded");
		}
	}
	if (checkDuplicationStr(listStatusCode) == false)
		throw std::runtime_error("Error_page's list of status code duplicated");
	_errPage.listError = listStatusCode;
	if (checkIfFileAvailabe(vect[i], O_RDONLY) == false)
		throw std::runtime_error("Error_page file can't be reached");
	if (checkFileExtension(vect[i], ".html") == false)
		throw std::runtime_error("Error page need to be an html file");
	_errPage.filePath = vect[i];
}

/* [?] : Size value per units need to be limited and be > 0? */
void MyConf::parseSize(stringVect const & vect)
{
	float size;

	if (vect.size() != 2)
		throw std::runtime_error("Size contains only two values");
	_size = vect;
	try
	{
		size = convertToBytes(_size);
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error("The value or unit of the size element is invalid");
	}
	if (size < 0)
		throw std::runtime_error("Size need to be a positive number");
	_maxSize =  size;
}

/* [!] : Eliminate double // */
void MyConf::parseLocationBloc(vectDataIt & itUpper, vectData & data)
{
	std::string	locValue = (*itUpper).value[0];

	if ((locValue[0] != '/') 
			|| (locValue.size() > 1 && locValue[locValue.size() - 1] == '/')
			|| locValue.find("//") != std::string::npos)
		throw std::runtime_error("Syntax incorrect for location name");
	MyLoc loc(locValue);
	loc.parseLocation(data);
	_location.push_back(loc);
}

MyConf::MyConf(){_hasErrPage = false;}

MyConf::MyConf(MyConf const & other) : AConfig(other){}

MyConf &MyConf::operator=(MyConf const &other)
{
    if (this != & other){AConfig::operator=(other);}
	return *this;
}

MyConf::~MyConf(){}

bool MyConf::getStatus(){return _checkStats;}

void MyConf::setHostName(std::string const & host)
{
	if (host.size())
		this->_host = host;
}

void MyConf::init_port_child(int const & port)
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

void MyConf::parseConfig(vectData & data, intVect& allPort)
{
	std::string elt;
	vectData	locationData;
	vectDataIt	itUpper;
	bool		collectData = false;

	checkConfigElement(data);
	for(vectDataIt it = data.begin(); it != data.end(); it++)
	{
		elt = (*it).key;
		if ((*it).lvl == 1 && collectData == true)
		{
			parseLocationBloc(itUpper, locationData);
			collectData = false;
			locationData.clear();
		}
		if (elt == "host")
			parseHost((*it).value);
		else if (elt == "port")
			parsePort((*it).value, allPort);
		else if (elt == "error_page")
			parseErrorPage((*it).value);
		else if (elt == "size")
			parseSize((*it).value);
		else if (elt == "location")
		{
			if ((*it).lvl != 1)
				throw std::runtime_error("Location's indentation is wrong");
			itUpper = it;
			collectData = true;
		}
		else if ((*it).lvl == 2)
			locationData.push_back(*it);
		else
			throw std::runtime_error(elt + " is not part of the server's elements");
	}
	if (collectData == true)
		parseLocationBloc(itUpper, locationData);
	manageUploadLocation();
}

void	MyConf::displayConfig() const
{
	std::cout << "------------------------------------------" << std::endl;
	std::cout << "Host " << _host << std::endl;
	std::cout << "Port ";
	for (intVectIt itInt = _port.begin(); itInt != _port.end(); itInt++)
		std::cout << " " << (*itInt);
	std::cout << std::endl;
	std::cout << "size " << _maxSize << " bytes" << std::endl;
	if (_errPage.listError.size())
	{
		std::cout << "error_page ";
		displayStringVect(_errPage.listError);
		std::cout << _errPage.filePath << std::endl;
	}
	for (vectLocationcIt it = _location.begin(); it != _location.end(); it++)
	{
		(*it).displayLocation();
		std::cout << std::endl;
	}
}
