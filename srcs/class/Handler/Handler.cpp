/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:36 by barandri          #+#    #+#             */
/*   Updated: 2025/09/18 08:48:56 by barandri         ###   ########.fr       */
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
		if ((*itFile)->getFileFd() == fd)
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

vectServerIt Handler::getServer(int const & i)
{
	vectServerIt it = _vectServer.begin() + i;

    return it;
}

VClientIt Handler::getClient(int const & i)
{
	VClientIt	it = _vectClient.begin() + i;

    return it;
}

vectFileFdIt Handler::getFileFd(int const &i)
{
	vectFileFdIt it = _vectFileFd.begin() + i;

    return it;
}

void Handler::setVectServer(vectServer &vectServer)
{
	_vectServer = vectServer;
}

void Handler::addToVectClient(Server const & server, int const & fd)
{
	_vectClient.push_back(Client(server, fd, true));
}

// void Handler::addToVectFileFd(int const & fd, int const & fdCli, std::string const & filePath)
// {
// 	FileFd fileFd(fd, fdCli, filePath);

// 	fileFd.setFileType();
// 	fileFd.setupFd();
// 	_vectFileFd.push_back(fileFd);
// }

void Handler::addToVectFileFd(FileFd *fileFd)
{
	fileFd->setFileType();
	fileFd->setupFd();
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

void	Handler::closeAllFd()
{
	FileFd::closeSockFd();
	for (vectServerIt itVect = _vectServer.begin(); itVect != _vectServer.end(); itVect++)
		close((*itVect).get_fdSocket());
	for (VClientIt itCli = _vectClient.begin(); itCli != _vectClient.end(); itCli++)
		close((*itCli).get_clientFD());
	for (vectFileFdIt itFile = _vectFileFd.begin(); itFile != _vectFileFd.end(); itFile++)
		close((*itFile)->getFileFd());
}

void Handler::displayHandlerStat()
{
	std::cout << "========================================== \n";
	std::cout << "		HANDLER STATS \n";
	std::cout << "Nb Server = " << _vectServer.size() << " [";
	for(size_t i = 0; i < _vectServer.size(); i++)
		std::cout << _vectServer[i].get_fdSocket() << " ";
	std::cout << "]" << std::endl;
	std::cout << std::endl;
	std::cout << "Nb Client = " << _vectClient.size() << " [";
	for(size_t i = 0; i < _vectClient.size(); i++)
		std::cout << _vectClient[i].get_clientFD() << " ";
	std::cout << "]" << std::endl;
	std::cout << std::endl;
	std::cout << "Nb FileFd = " << _vectFileFd.size() << " [";
	for(size_t i = 0; i < _vectFileFd.size(); i++)
		std::cout << _vectFileFd[i]->getFileFd() << " ";
	std::cout << "]" << std::endl;
	std::cout << std::endl;
	std::cout << "========================================== \n";
}
