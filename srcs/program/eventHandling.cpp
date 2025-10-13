/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/10/13 11:59:32 by rsafidy-         ###   ########.fr       */
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
	std::string limit = "\r\n\r\n";
	std::string body;
	// Request		tmpRequest;
	std::string limiter = "--";
	std::string limiterEnd = "--";
	int			fdFromRequest[2] = {-1, -1};
	
	// if ((*it).getObjRequest().getMethod() == "POST" && (*it).getPtrFileFd() && (*it).getPtrFileFd()->get_endFilePost())
	// 	(*it).clearPtrFileFd();
	// tmpRequest = (*it).getObjRequest();
	(*it).receiveClientRequest();
	client_request = (*it).getClientRequest();
	// std::cout << "-->> \033[042m ManageRequestFromClient : request_size = " << client_request.size() << "\033[0m" << std::endl;
	if (client_request.size() == 0)
	{
		(*it).clearPtrFileFd();
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
		if ((*it).getCategoryResponse() == REDIR_RSP || (*it).getCategoryResponse() == LDIR_RSP)
		{
			epoll.epoll_update_event(fdCli, EPOLLOUT);
			return;
		}
	}
	// std::cout << (*it).getObjRequest().getIsCgi() << " => FdFromRequest = " << fdFromRequest[FD_STDOUT] 
	// << "/" << fdFromRequest[FD_STDERR] << std::endl;
	if ((*it).getObjRequest().getMethod() == "DELETE")
	{
		epoll.epoll_update_event(fdCli, EPOLLOUT);
		return;
	}
	if (fdFromRequest[FD_STDOUT] < 0)
	{
		if (fdFromRequest[FD_STDOUT] == -2)
		{
			std::cout << "BREAAAAAK\n";
			handler.setProgramBreak(true);
		}
		return;
	}

	if ((*it).getFdType() == HTML_FILE)
		handler.addToVectFileFd(it, fdFromRequest[FD_STDOUT]);
	else
	{
		handler.addToVectCgiFd(it, fdFromRequest[FD_STDOUT], fdFromRequest[FD_STDERR]);
		epoll.epoll_control(epollOpt, fdFromRequest[FD_STDERR]);
	}
	if ((*it).getPtrFileFd())
	{
		std::cout << " BODY ATO AAAANNN  ["<< (*it).getObjRequest().get_Body() << "]" << std::endl;
		(*it).getPtrFileFd()->set_post_body((*it).getObjRequest().get_Body());
		// std::cout << "		OAY EEEEEEEEEEEEE" << std::endl;
		// if (tmpRequest.getMethod() == "POST")
		// std::cout << "				\e[5mBODY\e[0m---["<< (*it).getobjREquest().get_v_body_two(0) << "]" << std::endl;
		// (*it).getFileFD()->setBuffer((*it).getobjREquest().get_v_body_two(0));
		// (*it).getobjREquest().remove_from_body_two(0);
	}
	epoll.epoll_control(epollOpt, fdFromRequest[FD_STDOUT]);
}

void	sendingResponseToClient(VClientIt & it, int & fdCli, EpollCreation & epoll)
{
	int			responseCategory = (*it).getCategoryResponse();
	std::cout << "-->> \033[45mSendingResponseToClient : " << responseCategory << "\033[0m" << std::endl;

	if (responseCategory == LDIR_RSP)
		sendLdirResponse(it, fdCli, epoll); //Review for Listing Directory : maybe sending multiple response if the string is not fully sent
	else if (responseCategory == REDIR_RSP || responseCategory == SINGLE_RSP) //Response send only one time : redirection, POST and DELETE method
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
			std::cout << "BODY---["<< body << "]\nBOUNDARY=["<< (*it)->get_boundary() <<"]" << std::endl;
			size_t len =  write((*it)->getFd(), body.c_str(), body.size());
			if (len == (size_t)(-1))
				std::cout << "BLEM->" << strerror(errno) << std::endl;
			else
				(*it)->setFileTotalSize(len);
			std::cout << "manoratra->" << (*it)->getFileTotalSize() << std::endl;
			(*it)->delete_range_body(0, _BUFFER);
		}
		// if (!(*it)->getBuffer().size() && ((*clIt).getObjRequest().get_c_length_received() == (*clIt).getObjRequest().get_content_length()))
		if (!(*it)->getBuffer().size())
		{
			std::cout << "			VITA TOMPOKO!" << std::endl;
			std::cout << "  TOTAL_LENGHT=" << (*clIt).getObjRequest().get_content_length() << "| RECEIVED=" << (*clIt).getObjRequest().get_c_length_received() << std::endl;
			(*it)->set_endFilePost(true);
			// epoll.epoll_update_event(ClientFD, EPOLLOUT);
			closeFileFdEvent(handler, i, epoll);
		}
	}
}

void	handleCgiEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx)
{
	vectCgiFdIt	it = handler.getCgiFdIterator(i);
	uint32_t	event = epoll.getEvent(eventIdx);
	int			fdCli;
	int			bytesRead;
	char		buffer[_BUFFER];
	std::string	bufferString;

	if (event & EPOLLOUT)
	{
		std::cout << "Must be EPOLLIN";
		return; //Error on event selection
	}
	if ((*it)->getStatus() == OP_START) //See if necessary
		return;
	std::cout << "\033[43mCgiFd EPOLLIN : " << (*it)->getFd() << " ; OP = " << (*it)->getStatus() << "\033[0m" << std::endl;
	bytesRead = read((*it)->getFd(), buffer, _BUFFER);
	if (bytesRead <= 0)
	{
		(*it)->setStatus(OP_CLOSE);
		(*it)->clearBuffer();
		closeCgiFdEvent(handler, i, epoll);
		return ;
	}
	bufferString.append(buffer, bytesRead);
	(*it)->setBuffer(bufferString);
	std::cout << "				******** Buffer CGI = \n" << bufferString << std::endl;
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

