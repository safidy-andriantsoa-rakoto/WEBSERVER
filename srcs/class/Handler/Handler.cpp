/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:36 by barandri          #+#    #+#             */
/*   Updated: 2025/10/01 10:37:19 by barandri         ###   ########.fr       */
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

int Handler::identifyFd(int const & fd, int & index)
{
	index = 0;
	for (vectServerIt itVect = _vectServer.begin(); itVect != _vectServer.end(); itVect++)
	{
		if ((*itVect).get_fdSocket() == fd)
			return SERVER;
		index++;
	}
	index = 0;
	for (VClientIt itCli = _vectClient.begin(); itCli != _vectClient.end(); itCli++)
	{
		if ((*itCli).get_clientFD() == fd)
			return CLIENT;
		index++;
	}
	index = 0;
	for (vectFileFdIt itFile = _vectFileFd.begin(); itFile != _vectFileFd.end(); itFile++)
	{
		if ((*itFile) && (*itFile)->getFileFd() == fd)
			return FILE_FD;
		index++;
	}
	return -1;
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

void Handler::addToVectFileFd(VClientIt & it, int const & fd, int const & fdCli, std::string const & filePath, int const & category)
{
	FileFd *fileFd = new FileFd(fd, fdCli, filePath, category);

	fileFd->setFileType();
	fileFd->setupFd();
	(*it).setFileFD(fileFd);
	_vectFileFd.push_back(fileFd);
}

vectFileFdIt	Handler::findFdInFileFd(vectFileFd & allFileFd, int const & fd)
{
	vectFileFdIt it;

	for(it = allFileFd.begin(); it != allFileFd.end(); it++)
	{
		if ((*it) && (*it)->getFileFd() == fd)
			break;
	}
	return it;
}

void Handler::removeClient(int const & index, EpollCreation & epoll)
{
	VClientIt		it = _vectClient.begin() + index;
	vectFileFdIt	itFd;
	int				fdFile;
	int				stockFdFile;

	std::cout << "REMOVING CLIENT " << index << std::endl;
	if ((*it).getFileFD())
	{
		fdFile = (*it).getFileFD()->getFileFd();
		stockFdFile = (*it).getFileFD()->getStockFd();
		itFd = findFdInFileFd(_vectFileFd, fdFile);
		epoll.epoll_clean(fdFile);
		if (stockFdFile != -1)
			close (stockFdFile);
		removeFileFd(itFd);
	}
	(*it).clearFileFd();
	_vectClient.erase(it);
	std::cout << "Removing client done\n";
}

void Handler::removeClient(VClientIt & it, EpollCreation & epoll)
{
	vectFileFdIt	itFd;
	int				fdFile;
	int				stockFdFile;

	std::cout << "REMOVING CLIENT FROM ITERATOR" << std::endl;
	if ((*it).getFileFD())
	{
		fdFile = (*it).getFileFD()->getFileFd();
		stockFdFile = (*it).getFileFD()->getStockFd();
		itFd = findFdInFileFd(_vectFileFd, fdFile);
		epoll.epoll_clean(fdFile);
		if (stockFdFile != -1)
			close (stockFdFile);
		removeFileFd(itFd);
	}
	(*it).clearFileFd();
	_vectClient.erase(it);
}

void Handler::removeFileFd(int const & index)
{
	std::cout << "REMOVING FILE FD " << index << std::endl;
	_vectFileFd.erase(_vectFileFd.begin() + index);
}

void Handler::removeFileFd(vectFileFdIt & it)
{
	std::cout << "REMOVING FILE FD FROM ITERATOR" << std::endl;
	_vectFileFd.erase(it);
}

void	Handler::cleanFdAndAlloc()
{
	FileFd::closeSockFd();
	for (vectServerIt itVect = _vectServer.begin(); itVect != _vectServer.end(); itVect++)
		(*itVect).clearAllFd();
	for (vectFileFdIt itFile = _vectFileFd.begin(); itFile != _vectFileFd.end(); itFile++)
	{
		if ((*itFile) && (*itFile)->getFileFd() != -1)
			close((*itFile)->getFileFd());
		if ((*itFile) && (*itFile)->getStockFd() != -1)
			close((*itFile)->getStockFd());
	}	
	for (VClientIt itCli = _vectClient.begin(); itCli != _vectClient.end(); itCli++)
	{
		if ((*itCli).get_clientFD() != -1)
			close((*itCli).get_clientFD());
		(*itCli).clearFileFd();
	}
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
		std::cout << _vectClient[i].get_clientFD() << ":" << _vectClient[i].getFileFD() << " ";
	std::cout << "]" << std::endl;
	std::cout << std::endl;
	std::cout << "Nb FileFd = " << _vectFileFd.size() << " [";
	for(size_t i = 0; i < _vectFileFd.size(); i++)
	{
		if (_vectFileFd[i])
			std::cout << _vectFileFd[i]->getFileFd() << ":" << _vectFileFd[i] << " ";
	}
	std::cout << "]" << std::endl;
	std::cout << std::endl;
	std::cout << "========================================== \n";
}
