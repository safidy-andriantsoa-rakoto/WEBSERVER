/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:36 by barandri          #+#    #+#             */
/*   Updated: 2025/10/27 14:53:52 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Handler.hpp"

Handler::Handler() : _programBreak(false), _isChildProcess(false){}

Handler::Handler(vectServer &vectServer) : _vectServer(vectServer), _programBreak(false), _isChildProcess(false){}

Handler::Handler(Handler const & other) :
	_vectServer(other._vectServer),
	_vectClient(other._vectClient), _programBreak(other._programBreak), _isChildProcess(other._isChildProcess){}

Handler &Handler::operator=(Handler const & other)
{
	if (this != &other)
	{
		_vectServer = other._vectServer;
		_vectClient = other._vectClient;
		_programBreak = other._programBreak;
		_isChildProcess = other._isChildProcess;
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
			throw ErrorExcept(500, "Internal Servor Error");
			return;
		}
		index++;
	}
	identifier = -1;
}

vectServer &Handler::getVectServer(){ return _vectServer;}

VClient &Handler::getVectClient(){ return _vectClient;}

vectCgiFd &Handler::getVectCgiFd(){ return _vectCgiFd;}

bool Handler::getProgramBreak() const {return _programBreak;}

void Handler::setProgramBreak(bool const &stat){_programBreak = stat;}

bool Handler::getIsChildProcess() const{return _isChildProcess;}

void Handler::setIsChildProcess(bool const &stat){ _isChildProcess = stat; }

vectServerIt Handler::getServerIterator(int const &i){return _vectServer.begin() + i;}

VClientIt Handler::getClientIterator(int const & i){return _vectClient.begin() + i;}

vectCgiFdIt Handler::getCgiFdIterator(int const &i){ return _vectCgiFd.begin() + i;}

void Handler::setVectServer(vectServer &vectServer){ _vectServer = vectServer;}

void Handler::addToVectClient(Server const & server, int const & fd)
{
	_vectClient.push_back(Client(server, fd, true));
}

void Handler::addToVectCgiFd(VClientIt &it, int *fd, pid_t const & childPid)
{
	int	fdCli = (*it).get_clientFD();
	CgiFd *cgiFd = new CgiFd(fd, childPid, fdCli);

	(*it).setPtrCgiFd(cgiFd);
	_vectCgiFd.push_back(cgiFd);
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

void Handler::removeClient(int const & index, EpollCreation & epoll)
{
	VClientIt		it = _vectClient.begin() + index;
	vectCgiFdIt		itCgi;
	int				cgiFd[3];

	std::cout << "REMOVING CLIENT " << index << std::endl;
	if ((*it).getPtrCgiFd())
	{
		cgiFd[FD_STDOUT] = (*it).getPtrCgiFd()->getOutputFd();
		cgiFd[FD_STDERR] = (*it).getPtrCgiFd()->getErrFd();
		cgiFd[FD_STDIN] = (*it).getPtrCgiFd()->getInputFd();
		itCgi = findFdInCgiFd(_vectCgiFd, cgiFd[FD_STDOUT]);
		for(int i = 0; i < 3; i++)
		{
			if (cgiFd[i] > 0)
				epoll.epoll_clean(cgiFd[i]);
		}
		removeCgiFd(itCgi);
		(*it).clearPtrCgiFd();
	}
	_vectClient.erase(it);
}

void Handler::removeClient(VClientIt & it, EpollCreation & epoll)
{
	vectCgiFdIt		itCgi;
	int				cgiFd[3];

	std::cout << "REMOVING CLIENT FROM ITERATOR" << std::endl;
	if ((*it).getPtrCgiFd())
	{
		cgiFd[FD_STDOUT] = (*it).getPtrCgiFd()->getOutputFd();
		cgiFd[FD_STDERR] = (*it).getPtrCgiFd()->getErrFd();
		cgiFd[FD_STDIN] = (*it).getPtrCgiFd()->getInputFd();
		itCgi = findFdInCgiFd(_vectCgiFd, cgiFd[FD_STDOUT]);
		for(int i = 0; i < 3; i++)
		{
			if (cgiFd[i] > 0)
				epoll.epoll_clean(cgiFd[i]);
		}
	}
	_vectClient.erase(it);
}

void	Handler::cleanFdAndAlloc()
{
	std::cout << "CLEAN FD AND ALLOC : " << _isChildProcess << std::endl;
	for (vectServerIt itVect = _vectServer.begin(); itVect != _vectServer.end(); itVect++)
		(*itVect).clearAllFd();
	for (vectCgiFdIt itCgiFd = _vectCgiFd.begin(); itCgiFd != _vectCgiFd.end(); itCgiFd++)
	{
		if ((*itCgiFd) && _isChildProcess == false)
		{
			if ((*itCgiFd)->getOutputFd() != -1)
				close((*itCgiFd)->getOutputFd());
			if ((*itCgiFd)->getInputFd() != -1)
				close((*itCgiFd)->getInputFd());
			if ((*itCgiFd)->getErrFd() != -1)
				close((*itCgiFd)->getErrFd());
		}
	}
	for (VClientIt itCli = _vectClient.begin(); itCli != _vectClient.end(); itCli++)
	{
		if ((*itCli).get_clientFD() != -1)
			close((*itCli).get_clientFD());
		(*itCli).clearPtrCgiFd();
	}
}

void Handler::displayHandlerStat()
{
	std::cout << "========================================== \n";
	std::cout << "		HANDLER STATS : " << _isChildProcess << std::endl;
	std::cout << "Nb Server = " << _vectServer.size() << " [";
	for(size_t i = 0; i < _vectServer.size(); i++)
		std::cout << _vectServer[i].get_fdSocket() << " ";
	std::cout << "]" << std::endl;
	std::cout << std::endl;
	std::cout << "Nb Client = " << _vectClient.size() << " [";
	for(size_t i = 0; i < _vectClient.size(); i++)
		std::cout << _vectClient[i].get_clientFD()  << ":" << _vectClient[i].getPtrCgiFd() << " ";
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

void Handler::check_client_timeout_all(EpollCreation& epoll)
{
	for (VClientIt it = _vectClient.begin(); it != _vectClient.end(); )
	{
		try
		{
			(*it).is_client_timeout();
			++it;
		}
		catch (const std::exception & e)
		{
			std::cerr << "			Error on client timeout check: " << e.what() << '\n';
			// it = _vectClient.erase(it);
			int fdrm = (*it).get_clientFD();
			this->removeClient(it, epoll);
			if (fdrm > -1)
				epoll.epoll_clean(fdrm);
			// this->removeClient(it,);
		}
	}
}
