/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyLoc.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:59:34 by barandri          #+#    #+#             */
/*   Updated: 2025/10/20 14:30:55 by trahanta         ###   ########.fr       */
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
            _location = elt;
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
    }
}

bool MyLoc::getStatus()
{
    return (_checkStats);
}

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
                if (name == "root")
                    _root.push_back(word);
                if (name == "autoindex")
                {
                    if (word == "ON")
                        _autoindex[0] = word;
                }
                if (name == "method")
                    _method.push_back(word);
                if (name == "redirect")
                    _redirect.push_back(word);
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
    index[index.size()] = _endConfig;
    if (currentBlock + 1 < index.size())
        end = index[currentBlock + 1];
    else
        end = file.size();
    getElementLocation(start, end, file);
    currentBlock++;
    _checkStats = true;
}
