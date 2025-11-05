/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyLoc.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:59:34 by barandri          #+#    #+#             */
/*   Updated: 2025/11/05 15:15:41 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MyLoc.hpp"
#include "ALocation.hpp"

MyLoc::MyLoc() {}

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

void MyLoc::myParseLocation(std::string const &line)
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
        {
            _location = elt;
            _locations = elt[0];
        }
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

bool MyLoc::getStatus(){return (_checkStats);}

void MyLoc::getElementLocation(const size_t &start, const size_t &end, const stringVect &file)
{
    for (size_t i = start; i < end; ++i)
    {
        std::istringstream iss(file[i]);
        std::string name, value;
        iss >> name;
        while (std::getline(iss, value))
        {
            std::istringstream s(value);
            std::string word;
            if (name == "location")
            {
                stringVect tab = splitString(value, " ");
                _locations = tab[0];
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
                    /*Add the cgi extension inside parsing : need two elt, ON/OFF + execution path*/
                    if (word == "ON" || word == "OFF")
                    {
                        _cgiExtension[0] = word;
                        /* Then Check for the execution path at _cgiExtension[1] */
                        _cgiExtension.push_back("/usr/bin/php-cgi"); //Temporary line
                    }
                    //tokony mbola misy erreur eto
                }
                if (name == "method")
                    _method.push_back(word);
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

void MyLoc::parseLocation(const stringVect &file)
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
    if (checkDuplicate2(file, "autoindex", start, end) || checkDuplicate(file, "root", start, end) || checkDuplicate(file, "method", start, end) || checkDuplicate(file, "cgi", start, end))
    {
        throw std::runtime_error("Error : duplicate setting find or not find setting");
    }
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
