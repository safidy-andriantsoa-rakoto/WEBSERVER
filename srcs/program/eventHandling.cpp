/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/10/29 15:18:05 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	handleServerEvent(Handler & handler, int const & i, EpollCreation & epoll)
{
	int 			fdc;
	vectServerIt	it = handler.getServerIterator(i);

	fdc = (*it).accept_client_connexion();
	epoll.epoll_control(EPOLLIN, fdc);
	handler.addToVectClient(handler.getVectServer()[i], fdc);
	std::cout << "\033[42mNEW CLIENT JOIN = " << fdc << "--> SERVER=" << (*it).get_fdSocket() << "\033[0m" << std::endl;
}

void	manageRequestFromClient(VClientIt & it, int & fdCli, EpollCreation & epoll, Handler & handler)
{
	uint32_t	epollOpt;
	std::string client_request;
	int			fdCgi[3] = {-1, -1, -1};
	pid_t		childPid;

	(*it).receiveClientRequest();
	client_request = (*it).getClientRequest();
	if (client_request.size() == 0)
		throw std::runtime_error("Request from client size = 0");
	try
	{
		(*it).processRequest(client_request, epollOpt, fdCgi, childPid);
	}
	catch(AExcept & e)
	{
		std::cerr << "\033[031mProcess Request exception catched : " << e._errMsg << "\033[0m" << std::endl;
		(*it).setCatchedResponse(e);
		epollOpt = EPOLLOUT;
	}
	if (hasSingleResponse((*it).getCategoryResponse()) == true)
	{
		(*it).setResponseStat(OP_ONCE);
		epollOpt = EPOLLOUT;
	}
	if ((*it).getObjRequest().getIsCgi() == true)
	{
		if (fdCgi[FD_STDOUT] == -2)
		{
			handler.setProgramBreak(true);
			handler.setIsChildProcess(true);
			return;
		}
		if (fdCgi[FD_STDOUT] > 0)
			createCgiFd(it, handler, epoll, fdCgi, childPid);
	}
	else
		epoll.epoll_update_event(fdCli, epollOpt);
}

void	sendingResponseToClient(VClientIt & it, int & fdCli, EpollCreation & epoll)
{
	std::cout << "-->> \033[45mSendingResponseToClient : " << (*it).getCategoryResponse() << "\033[0m" << std::endl;

	if ((*it).getPtrCgiFd())
		sendCgiResponse(it, fdCli, epoll);
	else
		sendStandardResponse(it, fdCli, epoll);
}

void	handleClientEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx)
{
	uint32_t	event = epoll.getEvent(eventIdx);
	VClientIt	it = handler.getClientIterator(i);
	int			fdCli = (*it).get_clientFD();
	
	if ((event & EPOLLIN) && (*it).getConnexionStat() == true)
		manageRequestFromClient(it, fdCli, epoll, handler);
	else if ((event & EPOLLOUT) && (*it).getConnexionStat() == true)
		sendingResponseToClient(it, fdCli, epoll);
	else
		throw std::runtime_error("HandleClientEvent error");
}

void	readCgiOutput(Handler & handler, int const & i, EpollCreation & epoll)
{
	int			bytesRead;
	char		buffer[_BUFFER];
	std::string	bufferString;
	vectCgiFdIt	it = handler.getCgiFdIterator(i);
	int			fdCli = (*it)->getClientFd();

	(*it)->checkTimeOut();
	if ((*it)->getReadStat() == OP_START)
		return;
	std::cout << "\033[43mCgiFd EPOLLIN : " << (*it)->getOutputFd() << " ; OP = " << (*it)->getReadStat() << "\033[0m" << std::endl;
	bytesRead = read((*it)->getOutputFd(), buffer, _BUFFER);
	if (!bytesRead)
	{
		std::cout << "Nothing to read from CGI OUTPUT : " << bytesRead << std::endl;
		(*it)->setReadStat(OP_CLOSE);
		(*it)->clearBufferRead();
		epoll.epoll_clean((*it)->getOutputFd());
		(*it)->setOutputFd(-1);
		(*it)->setIsChildAlive(false);
		return ;
	}
	if (bytesRead == -1)
		throw ErrorExcept(500, "Read error on cgi");
	bufferString.append(buffer, bytesRead);
	(*it)->setBufferRead(bufferString);
	// std::cout << "				******** Buffer CGI = \n" << bufferString << std::endl;
	// std::cout << "				******** Buffer CGI END" << std::endl;
	(*it)->bufferReadParser();
	(*it)->checkCgiOutput();
	(*it)->displayBufferReadParsed();
	if ((*it)->getReadStat() == OP_OPEN)
		(*it)->setReadStat(OP_START);
	epoll.epoll_update_event(fdCli, EPOLLOUT);
}

void writeCgiInput(Handler &handler, int const &i, EpollCreation &epoll)
{
	int			bytesWritten;
	std::string	bufferString;
	vectCgiFdIt	it = handler.getCgiFdIterator(i);
	VClientIt	clIt = findFdInClient(handler.getVectClient(), (*it)->getClientFd());

	(*it)->checkChildProcess();
	if ((*it)->getIsChildAlive() == false)
	{
		epoll.epoll_clean((*it)->getInputFd());
		(*it)->setInputFd(-1);
		epoll.epoll_control(EPOLLIN, (*it)->getOutputFd());
		return;
	}
	std::cout << "\033[43mCgiFd EPOLLOUT : " << (*it)->getInputFd() << " ; OP = " << (*it)->getReadStat() << "\033[0m" << std::endl;
	bufferString = (*clIt).getObjRequest().getBody();
	// std::cout << "				******** Buffer CGI TO WRITE = \n" << bufferString << std::endl;
	// std::cout << "				******** Buffer CGI END TO WRITE" << std::endl;
	bytesWritten = write((*it)->getInputFd(), bufferString.c_str(), bufferString.size());
	std::cout << "Bytewritten = " << bytesWritten << std::endl;
	if (!bytesWritten)
	{
		epoll.epoll_clean((*it)->getInputFd());
		(*it)->setInputFd(-1);
		return ;
	}
	if (bytesWritten == -1)
	{
		epoll.epoll_clean((*it)->getInputFd());
		(*it)->setInputFd(-1);
		throw ErrorExcept(500, "Write CGI error");
	}
	if ((*clIt).getObjRequest().checkContentReceived() == true)
	{
		std::cout << "		Ending writing" << std::endl;
		(*it)->setWriteStat(OP_CLOSE);
		epoll.epoll_clean((*it)->getInputFd());
		(*it)->setInputFd(-1);
		epoll.epoll_control(EPOLLIN, (*it)->getOutputFd());
	}
	epoll.epoll_update_event((*it)->getClientFd(), EPOLLIN);
}
