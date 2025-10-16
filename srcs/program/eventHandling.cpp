/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/10/16 13:32:11 by rsafidy-         ###   ########.fr       */
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
	std::string	content;
	int			fdFromRequest[3] = {-1, -1, -1};

	(*it).receiveClientRequest();
	client_request = (*it).getClientRequest();
	if (client_request.size() == 0)
	{
		std::cout << "			Request size = " << client_request.size() << std::endl;
		handler.checkAndRemoveCgiFd((*it).getPtrCgiFd());
		(*it).clearPtrCgiFd();
		return ;
		// throw std::runtime_error("ManageRequestFromClient - request size = 0"); // mila timing ohatra ny philo
	}
	try
	{
		(*it).processRequest(client_request, epollOpt, fdFromRequest, content);
	}
	catch(AExcept & e)
	{
		std::cerr << "\033[031mProcess Request exception catched : " << e._errMsg << "\033[0m" << std::endl;
		(*it).setCatchedResponse(e);
		epollOpt = EPOLLOUT;
	}
	if (hasSingleResponse((*it).getCategoryResponse()) == true)
		epoll.epoll_update_event(fdCli, EPOLLOUT);
	else if ((*it).getObjRequest().getIsCgi() == true)
		addFdInHandlerAndEpoll(it, handler, epoll, epollOpt, fdFromRequest);
	else
	{
		std::cout << "ATY EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << std::endl;
		if ((*it).getCategoryResponse() == STD_RSP)
			(*it).setStandardResponse(content);
		epoll.epoll_update_event(fdCli, epollOpt);
	}
}

void	sendingResponseToClient(VClientIt & it, int & fdCli, EpollCreation & epoll)
{
	std::cout << "-->> \033[45mSendingResponseToClient : " << (*it).getCategoryResponse() << "\033[0m" << std::endl;

	if (hasSingleResponse((*it).getCategoryResponse()))
		sendSingleResponse(it, fdCli, epoll);
	else if ((*it).getPtrCgiFd())
		sendCgiResponse(it, fdCli, epoll);
	else
		sendHtmlFileResponse(it, fdCli, epoll);
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

/*
	Cgi event is only reading through pipe no matter the method is
*/
void	readCgiOutput(Handler & handler, int const & i, EpollCreation & epoll)
{
	int			fdCli;
	int			bytesRead;
	char		buffer[_BUFFER];
	std::string	bufferString;
	vectCgiFdIt	it = handler.getCgiFdIterator(i);


	if ((*it)->getStatus() == OP_START) //See if necessary
		return;
	std::cout << "\033[43mCgiFd EPOLLIN : " << (*it)->getOutputFd() << " ; OP = " << (*it)->getStatus() << "\033[0m" << std::endl;
	bytesRead = read((*it)->getOutputFd(), buffer, _BUFFER);
	if (bytesRead <= 0)
	{
		(*it)->setStatus(OP_CLOSE);
		(*it)->clearBuffer();
		// closeCgiFdEvent(handler, i, epoll, (*it)->getOutputFd());
		epoll.epoll_clean((*it)->getOutputFd());
		return ;
	}
	bufferString.append(buffer, bytesRead);
	(*it)->setBuffer(bufferString);
	std::cout << "				******** Buffer CGI = \n" << bufferString.size() << std::endl;
	(*it)->bufferParser();
	(*it)->checkError();
	// (*it)->displayBufferParsed();
	if ((*it)->getStatus() == OP_OPEN)
	{
		(*it)->setStatus(OP_START);
		fdCli = (*it)->getClientFd();
		epoll.epoll_update_event(fdCli, EPOLLOUT);
	}
}

void writeCgiInput(Handler &handler, int const &i, EpollCreation &epoll)
{
	int			bytesWritten;
	std::string	bufferString;
	vectCgiFdIt	it = handler.getCgiFdIterator(i);
	VClientIt	clIt = findFdInClient(handler.getVectClient(), (*it)->getClientFd());

	bufferString = (*clIt).getObjRequest().getBody();
	std::cout << "\033[43mCgiFd EPOLLOUT : " << (*it)->getOutputFd() << " ; OP = " << (*it)->getStatus() << "\033[0m" << std::endl;
	// std::cout << "Buffer = " << bufferString << " => " << bufferString.size() << std::endl;
	bytesWritten = write((*it)->getInputFd(), bufferString.c_str(), bufferString.size());
	if (bytesWritten <= 0)
	{
		std::cout << "Bytewritten = " << bytesWritten << std::endl;
		return ;
	}
	// (*it)->setStatus(OP_CLOSE);
	// (*it)->clearBuffer();
	epoll.epoll_clean((*it)->getInputFd());
	epoll.epoll_control(EPOLLIN, (*it)->getOutputFd());
	epoll.epoll_control(EPOLLIN, (*it)->getErrFd());
}
