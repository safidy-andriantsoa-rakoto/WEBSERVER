/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/10/15 15:52:41 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void handleServerEvent(Handler &handler, int const &i, EpollCreation &epoll)
{
	int fdc;
	vectServerIt it = handler.getServerIterator(i);

	fdc = (*it).accept_client_connexion();
	epoll.epoll_control(EPOLLIN, fdc);
	handler.addToVectClient(handler.getVectServer()[i], fdc);
	std::cout << "\033[42mNEW CLIENT JOIN = " << fdc << "--> SERVER=" << (*it).get_fdSocket() << "\033[0m" << std::endl;
}

void manageRequestFromClient(VClientIt &it, int &fdCli, EpollCreation &epoll, Handler &handler)
{
	uint32_t epollOpt;
	std::string client_request;
	std::string limit = "\r\n\r\n";
	std::string body;
	// Request		tmpRequest;
	std::string limiter = "--";
	std::string limiterEnd = "--";
	int fdFromRequest[2] = {-1, -1};

	(*it).receiveClientRequest();
	client_request = (*it).getClientRequest();
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
	catch (AExcept &e)
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
			return;
		}
	}
	if ((*it).getFdType() == HTML_FILE)
	{
		handler.addToVectFileFd(it, fdFromRequest[FD_STDOUT]);
		epoll.epoll_control(epollOpt, fdFromRequest[FD_STDOUT]);
	}
	else if ((*it).getFdType() == POST_FILE && fdFromRequest[FD_STDOUT] != -1)
	{
		handler.addToVectFileFd(it, fdFromRequest[FD_STDOUT]);
		epoll.epoll_control(epollOpt, fdFromRequest[FD_STDOUT]);
		if (!(*it).getBoundaryLimit().empty())
			(*it).getPtrFileFd()->set_boundary("--" + (*it).getBoundaryLimit());
	}
	else if ((*it).getObjRequest().getIsCgi() == true && fdFromRequest[FD_STDOUT] != -1 && fdFromRequest[FD_STDERR] != -1) // verifierna -2 ou -1?
	{
		std::cout << "\n\n"
				  << "		CREATE CGIFD *\n\n"
				  << std::endl;
		handler.addToVectCgiFd(it, fdFromRequest[FD_STDOUT], fdFromRequest[FD_STDERR]);
		epoll.epoll_control(epollOpt, fdFromRequest[FD_STDERR]);
	}
	if ((*it).getPtrFileFd())
		(*it).getPtrFileFd()->set_post_body((*it).getObjRequest().get_Body());
}

void sendingResponseToClient(VClientIt &it, int &fdCli, EpollCreation &epoll)
{
	int responseCategory = (*it).getCategoryResponse();
	std::cout << "-->> \033[45mSendingResponseToClient : " << responseCategory << "\033[0m" << std::endl;

	if (responseCategory == LDIR_RSP)
		sendLdirResponse(it, fdCli, epoll);									  // Review for Listing Directory : maybe sending multiple response if the string is not fully sent
	else if (responseCategory == REDIR_RSP || responseCategory == SINGLE_RSP) // Response send only one time : redirection, POST and DELETE method
		sendSingleResponse(it, fdCli, epoll);
	else if ((*it).getPtrFileFd())
		sendHtmlFileResponse(it, fdCli, epoll);
	else if ((*it).getPtrCgiFd())
		sendCgiResponse(it, fdCli, epoll);
}

void handleClientEvent(Handler &handler, int const &i, EpollCreation &epoll, int const &eventIdx)
{
	uint32_t event = epoll.getEvent(eventIdx);
	VClientIt it = handler.getClientIterator(i);
	int fdCli = (*it).get_clientFD();

	if ((event & EPOLLIN) && (*it).getConnexionStat() == true)
		manageRequestFromClient(it, fdCli, epoll, handler);
	else if ((event & EPOLLOUT) && (*it).getConnexionStat() == true)
		sendingResponseToClient(it, fdCli, epoll);
	else
		throw std::runtime_error("HandleClientEvent error");
}

void readFromFileFd(vectFileFdIt &it, EpollCreation &epoll, Handler &handler, int const &i)
{
	int fdCli;
	int bytesRead;
	char buffer[_BUFFER];
	std::string bufferString;

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
		return;
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

void handleFileEvent(Handler &handler, int const &i, EpollCreation &epoll, int const &eventIdx)
{
	vectFileFdIt it = handler.getFileFdIterator(i);
	uint32_t event = epoll.getEvent(eventIdx);
	int ClientFD = (*it)->getClientFd();
	VClientIt clIt = findFdInClient(handler.getVectClient(), ClientFD);
	size_t data_rec = (*clIt).getObjRequest().get_c_length_received();
	size_t data_total = (*clIt).getObjRequest().get_content_length();
	size_t len;

	if (event & EPOLLIN)
			readFromFileFd(it, epoll, handler, i);
	else if (event & EPOLLOUT)
	{
		std::cout << "\n\nATY QQQ		\033[43mFD EPOLLOUT FOR FILE IO-" << "\033[0m" << (*it)->getFd() << std::endl;
		if ((*it)->getFd() != -1)
		{
			std::string body;
			size_t		pos;
			std::string boundary;
			(*it)->manageFd();
			
			body = (*it)->get_range_body(0, _BUFFER);
			boundary = "\r\n" + (*it)->get_boundary();
			pos = body.find(boundary);
			if (pos != std::string::npos)
				body = body.substr(0 , pos);
			std::cout << "  BODY FILEOUT->" << body << "<--" << std::endl;
		// 	boundary = (*it)->get_boundary();
		// 	if (boundary.size())
		// 	{
		// 		size_t pos = body.find(boundary);
		// 		if (pos != std::string::npos)
		// 			body.erase(0, boundary.size() + 2);
		// 		while (pos != std::string::npos)
		// 		{
		// 			std::string send;
 
		// 			pos = body.find(boundary);
		// 			if (pos != std::string::npos)
		// 			{
		// 				send = body.substr(0, pos);
		// 				body.erase(0, pos);
		// 				len = write((*it)->getFd(), send.c_str(), send.size());
		// 				if (len == (size_t)(-1))
		// 					std::cout << "BLEM->" << strerror(errno) << std::endl;
		// 				else
		// 					(*it)->setFileTotalSize(len);
		// 				body.erase(0, boundary.size() + 2);
		// 			}
		// 			else
		// 			{
		// 				len = write((*it)->getFd(), body.c_str(), body.size());
		// 				if (len == (size_t)(-1))
		// 					std::cout << "BLEM->" << strerror(errno) << std::endl;
		// 				else
		// 					(*it)->setFileTotalSize(len);
		// 				body.erase(0, body.size());
		// 			}
		// 		}
		// 		if (!body.size())
		// 			(*it)->delete_range_body(0, _BUFFER);
		// 		std::cout << "\n\n		ATY EEEE BOUND   \n" << std::endl;
		// 	}
		// 	else
		// 	{
			len = write((*it)->getFd(), body.c_str(), body.size());
			if (len == (size_t)(-1))
				std::cout << "BLEM->" << strerror(errno) << std::endl;
			else
				(*it)->setFileTotalSize(len);
			(*it)->delete_range_body(0, _BUFFER);
			// }
			// std::cout << "BODY---[" << body << "]\nBOUNDARY=[" << (*it)->get_boundary() << "]" << std::endl;
		}
		if (!(*it)->getBuffer().size() && data_rec == data_total)
		{
			std::cout << "			VITA TOMPOKO!" << std::endl;
			std::cout << "  TOTAL_LENGHT=" << (*clIt).getObjRequest().get_content_length() << "| RECEIVED=" << (*clIt).getObjRequest().get_c_length_received() << std::endl;
			(*it)->set_endFilePost(true);
			closeFileFdEvent(handler, i, epoll);
			// epoll.epoll_update_event(ClientFD, EPOLLOUT);
		}
	}
}

void handleCgiEvent(Handler &handler, int const &i, EpollCreation &epoll, int const &eventIdx)
{
	vectCgiFdIt it = handler.getCgiFdIterator(i);
	uint32_t event = epoll.getEvent(eventIdx);
	int fdCli;
	int bytesRead;
	char buffer[_BUFFER];
	std::string bufferString;

	if (event & EPOLLOUT)
	{
		std::cout << "Must be EPOLLIN";
		return; // Error on event selection
	}
	if ((*it)->getStatus() == OP_START) // See if necessary
		return;
	std::cout << "\033[43mCgiFd EPOLLIN : " << (*it)->getFd() << " ; OP = " << (*it)->getStatus() << "\033[0m" << std::endl;
	bytesRead = read((*it)->getFd(), buffer, _BUFFER);
	if (bytesRead <= 0)
	{
		(*it)->setStatus(OP_CLOSE);
		(*it)->clearBuffer();
		closeCgiFdEvent(handler, i, epoll);
		return;
	}
	bufferString.append(buffer, bytesRead);
	(*it)->setBuffer(bufferString);
	std::cout << "				******** Buffer CGI = \n"
			  << bufferString << std::endl;
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
