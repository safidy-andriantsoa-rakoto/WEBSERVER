/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:36 by barandri          #+#    #+#             */
/*   Updated: 2025/10/15 11:11:31 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Handler.hpp"

Handler::Handler() : _programBreak(false){}

Handler::Handler(vectServer &vectServer) : _vectServer(vectServer), _programBreak(false){}

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
		_programBreak = other._programBreak;
	}
	return *this;
}

Handler::~Handler(){}

void Handler::identifyFd(int const & fd, int & index, int & identifier)
{
	index = 0;
	for (vectServerIt itVect = _vectServer.begin(); itVect != _vectServer.end(); itVect++)
	{
		if ((*itVect).get_fdSocket() == fd)
		{
			identifier = SERVER;
			return;
		}
		index++;
	}
	index = 0;
	for (VClientIt itCli = _vectClient.begin(); itCli != _vectClient.end(); itCli++)
	{
		if ((*itCli).get_clientFD() == fd)
		{
			identifier = CLIENT;
			return;
		}
		index++;
	}
	index = 0;
	for (vectFileFdIt itFile = _vectFileFd.begin(); itFile != _vectFileFd.end(); itFile++)
	{
		if ((*itFile) && (*itFile)->getFd() == fd)
		{
			identifier = FILE_FD;
			return;
		}
		index++;
	}
	index = 0;
	for (vectCgiFdIt itCgiFd = _vectCgiFd.begin(); itCgiFd != _vectCgiFd.end(); itCgiFd++)
	{
		if ((*itCgiFd) && (*itCgiFd)->getOutputFd() == fd)
		{
			identifier = CGI_FD_OUTPUT;
			return;
		}
		if ((*itCgiFd) && (*itCgiFd)->getInputFd() == fd)
		{
			identifier = CGI_FD_INPUT;
			return;
		}
		if ((*itCgiFd) && (*itCgiFd)->getErrFd() == fd)
		{
			(*itCgiFd)->setIsError(true);
			identifier = CGI_FD_ERR;
			throw ErrorExcept(500, "Bad gateway");
		}
		index++;
	}
	identifier = -1;
}

vectServer &Handler::getVectServer(){ return _vectServer;}

VClient &Handler::getVectClient(){ return _vectClient;}

vectFileFd &Handler::getVectFileFd(){ return _vectFileFd;}

vectCgiFd &Handler::getVectCgiFd(){ return _vectCgiFd;}

bool Handler::getProgramBreak() {return _programBreak;}

void Handler::setProgramBreak(bool const &stat){_programBreak = stat;}

vectServerIt Handler::getServerIterator(int const &i){return _vectServer.begin() + i;}

VClientIt Handler::getClientIterator(int const & i){return _vectClient.begin() + i;}

vectFileFdIt Handler::getFileFdIterator(int const &i){return _vectFileFd.begin() + i;}

vectCgiFdIt Handler::getCgiFdIterator(int const &i){ return _vectCgiFd.begin() + i;}

void Handler::setVectServer(vectServer &vectServer){ _vectServer = vectServer;}

void Handler::addToVectClient(Server const & server, int const & fd)
{
	_vectClient.push_back(Client(server, fd, true));
}

void Handler::addToVectFileFd(VClientIt & it, int const & fd)
{
	int			fdCli = (*it).get_clientFD();
	std::string	filePath = (*it).getObjRequest().getRequestTarget();
	FileFd 		*fileFd = new FileFd(fd, fdCli, filePath);

	fileFd->setFileType();
	fileFd->setupFd();
	(*it).setPtrFileFd(fileFd);
	_vectFileFd.push_back(fileFd);
}

void Handler::addToVectCgiFd(VClientIt &it, int const &fdOut, int const & fdErr, int const & fdIn)
{
	int	fdCli = (*it).get_clientFD();
	CgiFd *cgiFd = new CgiFd(fdOut, fdErr, fdIn, fdCli);

	(*it).setPtrCgiFd(cgiFd);
	_vectCgiFd.push_back(cgiFd);
}

vectFileFdIt	Handler::findFdInFileFd(vectFileFd & allFileFd, int const & fd)
{
	vectFileFdIt it;

	for(it = allFileFd.begin(); it != allFileFd.end(); it++)
	{
		if ((*it) && (*it)->getFd() == fd)
			break;
	}
	return it;
}

vectFileFdIt Handler::findPtrInFileFd(vectFileFd &allFileFd, FileFd *ptr)
{
	vectFileFdIt it;

	for(it = allFileFd.begin(); it != allFileFd.end(); it++)
	{
		if ((*it) == ptr)
			break;
	}
	return it;
}

vectCgiFdIt Handler::findFdInCgiFd(vectCgiFd & allCgiFd, int const & fd)
{
	vectCgiFdIt it;

	for(it = allCgiFd.begin(); it != allCgiFd.end(); it++)
	{
		if ((*it) && (*it)->getOutputFd() == fd)
			break;
	}
	return it;
}

vectCgiFdIt Handler::findPtrInCgiFd(vectCgiFd &allCgiFd, CgiFd *ptr)
{
	vectCgiFdIt it;

	for(it = allCgiFd.begin(); it != allCgiFd.end(); it++)
	{
		if ((*it) == ptr)
			break;
	}
	return it;
}

void Handler::removeCgiFd(int const &index)
{
	std::cout << "RemoveCgiFd :" << index << std::endl;
	_vectCgiFd.erase(_vectCgiFd.begin() + index);
}

void Handler::removeCgiFd(vectCgiFdIt &it)
{
	std::cout << "RemoveCgiFd from iterator:" << std::endl;
	_vectCgiFd.erase(it);
}

void Handler::checkAndRemoveCgiFd(CgiFd *ptr)
{
	vectCgiFdIt	it = findPtrInCgiFd(_vectCgiFd, ptr);

	if (it != _vectCgiFd.end())
		removeCgiFd(it);
}

void Handler::removeClient(int const & index, EpollCreation & epoll)
{
	VClientIt		it = _vectClient.begin() + index;
	vectFileFdIt	itFd;
	vectCgiFdIt		itCgi;
	int				fdFile;
	int				stockFdFile;
	int				cgiFd;

	std::cout << "REMOVING CLIENT " << index << std::endl;
	if ((*it).getPtrFileFd())
	{
		fdFile = (*it).getPtrFileFd()->getFd();
		stockFdFile = (*it).getPtrFileFd()->getStockFd();
		itFd = findFdInFileFd(_vectFileFd, fdFile);
		epoll.epoll_clean(fdFile);
		if (stockFdFile != -1)
			close (stockFdFile);
		removeFileFd(itFd);
	}
	if ((*it).getPtrCgiFd())
	{
		cgiFd = (*it).getPtrCgiFd()->getOutputFd();
		itCgi = findFdInCgiFd(_vectCgiFd, cgiFd);
		epoll.epoll_clean(fdFile);
		removeCgiFd(itCgi);
	}
	_vectClient.erase(it);
}

void Handler::removeClient(VClientIt & it, EpollCreation & epoll)
{
	vectFileFdIt	itFd;
	int				fdFile;
	int				stockFdFile;

	std::cout << "REMOVING CLIENT FROM ITERATOR" << std::endl;
	if ((*it).getPtrFileFd())
	{
		fdFile = (*it).getPtrFileFd()->getFd();
		stockFdFile = (*it).getPtrFileFd()->getStockFd();
		itFd = findFdInFileFd(_vectFileFd, fdFile);
		epoll.epoll_clean(fdFile);
		if (stockFdFile != -1)
			close (stockFdFile);
		removeFileFd(itFd);
	}
	(*it).clearPtrFileFd();
	_vectClient.erase(it);
}

void Handler::removeFileFd(int const & index)
{
	_vectFileFd.erase(_vectFileFd.begin() + index);
}

void Handler::removeFileFd(vectFileFdIt & it)
{
	_vectFileFd.erase(it);
}

void Handler::checkAndRemoveFileFd(FileFd *ptr)
{
	vectFileFdIt	it = findPtrInFileFd(_vectFileFd, ptr);

	if (it != _vectFileFd.end())
		removeFileFd(it);
}

void	Handler::cleanFdAndAlloc()
{
	FileFd::closeSockFd();
	for (vectServerIt itVect = _vectServer.begin(); itVect != _vectServer.end(); itVect++)
		(*itVect).clearAllFd();
	for (vectFileFdIt itFile = _vectFileFd.begin(); itFile != _vectFileFd.end(); itFile++)
	{
		if ((*itFile) && (*itFile)->getFd() != -1)
			close((*itFile)->getFd());
		if ((*itFile) && (*itFile)->getStockFd() != -1)
			close((*itFile)->getStockFd());
	}
	for (vectCgiFdIt itCgiFd = _vectCgiFd.begin(); itCgiFd != _vectCgiFd.end(); itCgiFd++)
	{
		if ((*itCgiFd) && (*itCgiFd)->getOutputFd() != -1)
			close((*itCgiFd)->getOutputFd());
		if ((*itCgiFd) && (*itCgiFd)->getInputFd() != -1)
			close((*itCgiFd)->getInputFd());
		if ((*itCgiFd) && (*itCgiFd)->getErrFd() != -1)
			close((*itCgiFd)->getErrFd());
	}
	for (VClientIt itCli = _vectClient.begin(); itCli != _vectClient.end(); itCli++)
	{
		if ((*itCli).get_clientFD() != -1)
			close((*itCli).get_clientFD());
		(*itCli).clearPtrFileFd();
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
		std::cout << _vectClient[i].get_clientFD() << ":" << _vectClient[i].getPtrFileFd() << ":" << _vectClient[i].getPtrCgiFd() << " ";
	std::cout << "]" << std::endl;
	std::cout << std::endl;
	std::cout << "Nb FileFd = " << _vectFileFd.size() << " [";
	for(size_t i = 0; i < _vectFileFd.size(); i++)
	{
		if (_vectFileFd[i])
			std::cout << _vectFileFd[i]->getFd() << ":" << _vectFileFd[i] << " ";
	}
	std::cout << "]" << std::endl;
	std::cout << std::endl;
	std::cout << "Nb CgiFd = " << _vectCgiFd.size() << " [";
	for(size_t i = 0; i < _vectCgiFd.size(); i++)
	{
		if (_vectCgiFd[i])
			std::cout << _vectCgiFd[i]->getInputFd() << "|" << _vectCgiFd[i]->getOutputFd() << "|" << _vectCgiFd[i]->getErrFd() << ":" << _vectCgiFd[i] << " ";
	}
	std::cout << "]" << std::endl;
	std::cout << std::endl;
	std::cout << "========================================== \n";
}
