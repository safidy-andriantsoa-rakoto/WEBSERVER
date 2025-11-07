/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyLoc.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:59:34 by barandri          #+#    #+#             */
/*   Updated: 2025/11/07 11:11:50 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MyLoc.hpp"
#include "ALocation.hpp"

void MyLoc::checkLocElement(vectData & data)
{
    stringVect  elt;

    for(vectDataIt it = data.begin(); it != data.end(); it++)
    {
        if ((*it).value.size() == 0)
            throw std::runtime_error("Location's minimal element requirement not respected");
        elt.push_back((*it).key);
    }
    if ((std::find(elt.begin(), elt.end(), "redirect") != elt.end()) && elt.size() != 1)
        throw std::runtime_error("redirect go alone inside a location bloc");
    if (checkDuplicateInStringVect(elt) == true)
        throw std::runtime_error("Location element duplicated");
    if (elt[0] != "redirect")
        checkMandatoryElement(elt);
}

void MyLoc::checkMandatoryElement(stringVect &vect)
{
    stringVect  mandatory;

    mandatory.push_back("root");
    mandatory.push_back("method");
    if (findEltInStringVect(vect, mandatory) == false)
        throw std::runtime_error("root and method are mandatory for location element");
}

void MyLoc::addLocationBySplitParsing(std::string const &line)
{
    stringVect attr;
    stringVect elt;
    std::string topic;

    attr = splitString(line, " | ");
    for (stringVectIt it = attr.begin(); it != attr.end(); it++)
    {
        elt = splitString(*it, " ");
        topic = elt[0];
        elt.erase(elt.begin());
        if (topic == "location")
            _name = elt[0];
        if (topic == "root")
            _root = elt;
        if (topic == "autoindex")
            _autoindex = elt;
        if (topic == "method")
            _method = elt;
        if (topic == "redirect")
            _redirect = elt;
        if (topic == "index")
            _index = elt;
        if (topic == "cgi")
            _cgiExtension = elt;
    }
}

void MyLoc::parseRoot(stringVect & vect)
{
    if (vect.size() != 1)
        throw std::runtime_error("Root need  one element");
    if (checkIfDirectoryAvailable(vect[0]) == false)
        throw std::runtime_error("Root is not a directory or can't be reached");
    _root = vect;
}

void MyLoc::parseMethod(stringVect & vect)
{
    if (vect.size() > 3)
        throw std::runtime_error("The maximum of method allowed is 3");
    if (checkValueMethod(vect) == false)
        throw std::runtime_error("Server supports only GET, POST, DELETE");
    if (checkDuplicateInStringVect(vect) == true)
        throw std::runtime_error("Method are duplicated");
    _method = vect;
}

void MyLoc::parseAutoIndex(stringVect & vect)
{
    if (vect.size() != 1)
        throw std::runtime_error("Autoindex need 1 element");
    if (vect[0] != "ON" && vect[0] != "OFF")
        throw std::runtime_error("Autoindex can be activated by ON/OFF");
    _autoindex[0] = vect[0];
}

void MyLoc::parseCgiExt(stringVect & vect)
{
    bool    isCgi = false;

    if (vect.size() != 2)
        throw std::runtime_error("Cgi need 3 elements");
    if (vect[0] != "ON" && vect[0] != "OFF")
        throw std::runtime_error("Cgi can be activated by ON/OFF");
    if (vect[0] == "OFF" && vect.size() != 1)
        throw std::runtime_error("Cgi is OFF, no need for executable");
    if (vect[0] == "ON")
        isCgi = true;
    try
    {
        /* [!] : fstat or openRegFile ? */
        if (isCgi == true)
        {
            int fd = openRegFile(vect[1]);
            close(fd);
        }
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("Cgi executable is not valid");
    }
    _cgiExtension = vect;
}

void MyLoc::parseRedirect(stringVect & vect)
{
	size_t  i;

	if (vect.size() < 2)
		throw std::runtime_error("Redirect elements missing");
	for(i = 0; i < vect.size() - 1; i++)
	{
		if (vect[0] != "30i" && vect[0] != "302" && vect[0] != "307" && vect[0] != "308")
			throw std::runtime_error("Invalid status code for redirection");
	}
    _redirect = vect;
}

MyLoc::MyLoc(){}

MyLoc::MyLoc(std::string const &locName): ALocation(locName){}

MyLoc::MyLoc(MyLoc const &other) : ALocation(other) {}

MyLoc &MyLoc::operator=(MyLoc const &other)
{
    if (this != &other)
    {
        ALocation::operator=(other);
    }
    return *this;
}

MyLoc::~MyLoc() {}

bool MyLoc::getStatus(){return (_checkStats);}

void MyLoc::getElementLocation(const size_t &start, const size_t &end, const stringVect &file)
{
    for (size_t i = start; i < end; ++i)
    {
        std::istringstream iss(file[i]);
        std::string name, value;
        bool    cgiOn(false); /* [UPDATE] */
        iss >> name;
        while (std::getline(iss, value))
        {
            std::istringstream s(value);
            std::string word;
            if (name == "location")
            {
                stringVect tab = splitString(value, " ");
                _name = tab[0];
            }
            while (s >> word)
            {
                if (name != "root" && name != "autoindex" && name != "root" && name != "method" && name != "redirect" && name != "location" && name !=  "cgi")
                    throw std::runtime_error("Error: invalid config->" + name);
                if (name == "root")
                    _root.push_back(word);
                if (name == "autoindex")
                {
                    if (word == "ON" || word == "OFF")
                        _autoindex[0] = word;
                    else
                        throw std::runtime_error("Error: invalid config->" + name);
                }
                if (name == "cgi")
                {
                    /* [UPDATE] */
                    if (cgiOn)
                        _cgiExtension.push_back(word);
                    if (word == "ON")
                    {
                        _cgiExtension[0] = word;
                        cgiOn = true;
                    }
                    //tokony mbola misy erreur eto
                }
                if (name == "method")
                {
                    if (word != "POST" && word != "GET" && word != "DELETE")
                        throw std::runtime_error("Error: Method not allowed->" + name + "=" + word);
                    _method.push_back(word);
                }
                if (name == "redirect")
                {
                    _redirect.push_back(word);
                    if (_redirect[0] == "301" || _redirect[0] == "302" || _redirect[0] == "307" || _redirect[0] == "308")
                    {
                        _redirection.error = atoi(_redirect[0].c_str());
                        try
                        {
                            _redirection.filePath = _redirect.at(1);
                        }
                        catch (const std::out_of_range &e)
                        {
                            std::cerr << "Error : " << e.what() << std::endl;
                        }
                    }
                    else
                        throw std::runtime_error("Error : redirection failed ");
                }
            }
        }
    }
}



void MyLoc::parseLocation(vectData & data)
{
    std::string  elt;

    checkLocElement(data);
    for(vectDataIt it = data.begin(); it != data.end(); it++)
    {
        elt = (*it).key;
        if (elt == "root")
            parseRoot((*it).value);
        else if (elt == "autoindex")
            parseAutoIndex((*it).value);
        else if (elt == "method")
            parseMethod((*it).value);
        else if (elt == "redirect")
            parseRedirect((*it).value);
        else if (elt == "cgi")
            parseCgiExt((*it).value);
        else
            throw std::runtime_error("Unknown element in location bloc");
    }
}

void MyLoc::oldParseLocation(stringVect const &file)
{
    static intVect index = containsExactWordInVector(file, "location");
    static size_t currentBlock = 0;
    if (currentBlock >= index.size())
        return;
    size_t start = index[currentBlock];
    size_t end;
    if (currentBlock + 1 < index.size())
        end = index[currentBlock + 1];
    else
        end = file.size();
    /* [!] : Add check for only redirect here !!!! */
    // if (checkDuplicate2(file, "autoindex", start, end) || checkDuplicate(file, "root", start, end) || checkDuplicate(file, "method", start, end) || checkDuplicate(file, "cgi", start, end))
    if (checkDuplicate(file, "root", start, end) || checkDuplicate(file, "method", start, end))
        throw std::runtime_error("Error : duplicate setting find or not find setting");
    try
    {
        getElementLocation(start, end, file);
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error(e.what());
    }
    currentBlock++;
    _checkStats = true;
}
