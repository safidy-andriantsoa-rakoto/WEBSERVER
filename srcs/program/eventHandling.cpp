/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/10/15 07:47:49 by barandri         ###   ########.fr       */
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
	int			fdFromRequest[3] = {-1, -1, -1};

	(*it).receiveClientRequest();
	client_request = (*it).getClientRequest();
	if (client_request.size() == 0)
	{
		std::cout << "			Request size = " << client_request.size() << std::endl;
		handler.checkAndRemoveFileFd((*it).getPtrFileFd());
		(*it).clearPtrFileFd();
		handler.checkAndRemoveCgiFd((*it).getPtrCgiFd());
		(*it).clearPtrCgiFd();
		throw std::runtime_error("ManageRequestFromClient - request size = 0");
	}
	try
	{
		(*it).processRequest(client_request, epollOpt, fdFromRequest);
	}
	catch(AExcept & e)
	{
		std::cerr << "\033[031mProcess Request exception catched : " << e._errMsg << "\033[0m" << std::endl;
		(*it).setCatchedResponse(e, fdFromRequest);
		epollOpt = EPOLLIN;
	}
	if (hasSingleResponse((*it).getCategoryResponse()) == true)
		epoll.epoll_update_event(fdCli, EPOLLOUT);
	else
		addFdInHandlerAndEpoll(it, handler, epoll, epollOpt, fdFromRequest);
}

void	sendingResponseToClient(VClientIt & it, int & fdCli, EpollCreation & epoll)
{
	std::cout << "-->> \033[45mSendingResponseToClient : " << (*it).getCategoryResponse() << "\033[0m" << std::endl;

	if (hasSingleResponse((*it).getCategoryResponse()))
		sendSingleResponse(it, fdCli, epoll);
	else if ((*it).getPtrFileFd())
		sendHtmlFileResponse(it, fdCli, epoll);
	else if ((*it).getPtrCgiFd())
		sendCgiResponse(it, fdCli, epoll);
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

void	readFromFileFd(vectFileFdIt & it, EpollCreation & epoll, Handler & handler, int const & i)
{
	int			fdCli;
	int			bytesRead;
	char		buffer[_BUFFER];
	std::string	bufferString;
	
	std::cout << "\033[43mFileFd EPOLLIN : " << (*it)->getClientFd() << " ; OP = " << (*it)->getStatus() << "\033[0m" << std::endl;
	(*it)->manageFd();
	if ((*it)->getStatus() == OP_START)
		return;
	bytesRead = read((*it)->getFd(), buffer, _BUFFER);
	if (bytesRead <= 0)
	{
		(*it)->setStatus(OP_CLOSE);
		(*it)->clearBuffer();
		closeFileFdEvent(handler, i, epoll);
		return ;
	}
	if ((*it)->getStatus() == OP_OPEN && (*it)->getFileTotalSize() <= static_cast<size_t>(bytesRead))
		(*it)->setStatus(OP_ONCE);
	bufferString.append(buffer, bytesRead);
	(*it)->setBuffer(bufferString);
	(*it)->restoreFd();
	if ((*it)->getStatus() == OP_ONGOING && ((*it)->getFileTotalSize() == (*it)->getFileSentSize() + static_cast<size_t>(bytesRead)))
		(*it)->setStatus(OP_CLOSE);
	if ((*it)->getStatus() == OP_ONCE || (*it)->getStatus() == OP_OPEN)
	{
		fdCli = (*it)->getClientFd();
		if ((*it)->getStatus() == OP_OPEN)
			(*it)->setStatus(OP_START);
		epoll.epoll_update_event(fdCli, EPOLLOUT);
	}
	if ((*it)->getStatus() == OP_ONCE || (*it)->getStatus() == OP_CLOSE)
		closeFileFdEvent(handler, i, epoll);
}

void	handleFileEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx)
{
	vectFileFdIt	it = handler.getFileFdIterator(i);
	uint32_t		event = epoll.getEvent(eventIdx);
	int				ClientFD = (*it)->getClientFd();
	VClientIt		clIt = findFdInClient(handler.getVectClient(), ClientFD);

	if (event & EPOLLIN)
		readFromFileFd(it, epoll, handler, i);
	else if (event & EPOLLOUT)
	{
		std::cout << "ATY QQQ		\033[43mFD EPOLLOUT FOR FILE IO-" << "\033[0m" << (*it)->getFd() << std::endl;
		if ((*it)->getFd() != -1)
		{
			std::string body;
			(*it)->manageFd();

			body = (*it)->get_range_body(0, _BUFFER);
			size_t len =  write((*it)->getFd(), body.c_str(), body.size());
			if (len == (size_t)(-1))
				std::cout << "BLEM->" << strerror(errno) << std::endl;
			else
				(*it)->setFileTotalSize(len);
			std::cout << "manoratra->" << (*it)->getFileTotalSize() << std::endl;
			(*it)->delete_range_body(0, _BUFFER);
		}
		if (!(*it)->getBuffer().size() && ((*clIt).getObjRequest().get_c_length_received() == (*clIt).getObjRequest().get_content_length()))
		{
			std::cout << "			VITA TOMPOKO!" << std::endl;
			std::cout << "  TOTAL_LENGHT=" << (*clIt).getObjRequest().get_content_length() << "| RECEIVED=" << (*clIt).getObjRequest().get_c_length_received() << std::endl;
			(*it)->set_endFilePost(true);
			// epoll.epoll_update_event(ClientFD, EPOLLOUT);
			closeFileFdEvent(handler, i, epoll);
		}
	}
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
		closeCgiFdEvent(handler, i, epoll);
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

/*
	Write request body to INPUT
*/
void writeCgiInput(Handler &handler, int const &i, EpollCreation &epoll)
{
	int			bytesWritten;
	// char		buffer[_BUFFER];
	std::string	bufferString;
	vectCgiFdIt	it = handler.getCgiFdIterator(i);
	VClientIt	clIt = findFdInClient(handler.getVectClient(), (*it)->getClientFd());

	/*
		Add condition to limit buffer < _BUFFER
		Check relation with CGI output, are they simultaneous? 
		or do we need to wait for write to complete before EPOLLIN FD_STDOUT?
	*/
	bufferString = (*clIt).getObjRequest().getBody();
	// strlcpy(buffer, bufferString.c_str(), bufferString.size());
	// if ((*it)->getStatus() == OP_START) //See if necessary
	// 	return;
	std::cout << "\033[43mCgiFd EPOLLOUT : " << (*it)->getOutputFd() << " ; OP = " << (*it)->getStatus() << "\033[0m" << std::endl;
	// std::cout << "Buffer = " << buffer << std::endl;
	bytesWritten = write((*it)->getInputFd(), bufferString.c_str(), _BUFFER);
	if (bytesWritten <= 0)
	{
		std::cout << "Bytewritten = " << bytesWritten << std::endl;
		(*it)->setStatus(OP_CLOSE);
		(*it)->clearBuffer();
		closeCgiFdEvent(handler, i, epoll);
		return ;
	}
	// if ((*it)->getStatus() == OP_OPEN)
	// {
	// 	(*it)->setStatus(OP_START);
	// 	// fdCli = (*it)->getClientFd();
	// }
}
