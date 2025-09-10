/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:36 by barandri          #+#    #+#             */
/*   Updated: 2025/09/10 13:05:05 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Handler.hpp"

Handler::Handler(){}

Handler::Handler(vectServer &vectServer) : _vectServer(vectServer){}

Handler::Handler(Handler const & other) :
	_vectServer(other._vectServer),
	_vectClient(other._vectClient),
	_vectFileFd(other._vectFileFd){}

Handler &Handler::operator=(Handler const & other)
{
	if (this != &other)
	{
		_vectServer = other._vectServer;
		_vectClient = other._vectClient;
		_vectFileFd = other._vectFileFd;
	}
	return *this;
}

Handler::~Handler(){}

std::string Handler::identifyFd(int const & fd, int & index)
{
	for (vectServerIt itVect = _vectServer.begin(); itVect != _vectServer.end(); itVect++)
	{
		if ((*itVect).get_fdSocket() == fd)
			return ("SERVER");
		index++;
	}
	index = 0;
	for (VClientIt itCli = _vectClient.begin(); itCli != _vectClient.end(); itCli++)
	{
		if ((*itCli).get_clientFD() == fd)
			return ("CLIENT");
		index++;
	}
	index = 0;
	for (vectFileFdIt itFile = _vectFileFd.begin(); itFile != _vectFileFd.end(); itFile++)
	{
		if ((*itFile).getFileFd() == fd)
			return ("FILEFD");
		index++;
	}
	throw std::runtime_error("Unknown fd");
	index = 0;
	return ("Unknown");
}

vectServer &Handler::getVectServer()
{
    return _vectServer;
}

VClient &Handler::getVectClient()
{
   return _vectClient;
}

vectFileFd &Handler::getVectFileFd()
{
    return _vectFileFd;
}

void Handler::addToVectClient(Server const & server, int const & fd)
{
	_vectClient.push_back(Client(server, fd));
}

void Handler::addToVectFileFd(int const & fd, int const & fdCli, std::string const & filePath)
{
	FileFd fileFd(fd, fdCli, filePath);

	fileFd.setFileType();
	fileFd.setupFd();
	_vectFileFd.push_back(fileFd);
}

void Handler::removeClient(int const & index)
{
	_vectClient.erase(_vectClient.begin() + index);
}

void Handler::removeFileFd(int const & index)
{
	_vectFileFd.erase(_vectFileFd.begin() + index);
}

void Handler::displayHandlerStat()
{
	std::cout << "========================================== \n";
	std::cout << "HANDLER STATS \n";
	std::cout << "Nb Server = " << _vectServer.size() << std::endl;
	for(size_t i = 0; i < _vectServer.size(); i++)
		std::cout << _vectServer[i].get_fdSocket() << " ";
	std::cout << std::endl;
	std::cout << "Nb Client = " << _vectClient.size() << std::endl;
	for(size_t i = 0; i < _vectClient.size(); i++)
		std::cout << _vectClient[i].get_clientFD() << " ";
	std::cout << std::endl;
	std::cout << "Nb FileFd = " << _vectFileFd.size() << std::endl;
	for(size_t i = 0; i < _vectFileFd.size(); i++)
		std::cout << _vectFileFd[i].getFileFd() << " ";
	std::cout << std::endl;
	std::cout << "========================================== \n";
}
