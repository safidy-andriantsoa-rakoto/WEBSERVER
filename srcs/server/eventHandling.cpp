/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/09/22 11:47:54 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	handleServerEvent(Handler & handler, int const & i, EpollCreation & epoll)
{
	int 			fdc;
	vectServerIt	it = handler.getServer(i);

	fdc = (*it).accept_client_connexion();
	epoll.epoll_control(EPOLLIN, fdc);
	handler.addToVectClient(handler.getVectServer()[i], fdc);
	std::cout << "\033[42mNEW CLIENT JOIN=\033[0m" << fdc << "-->\033[046mSERVER=" << (*it).get_fdSocket() << "\033[0m" << std::endl;
}

void	manageRequestFromClient(VClientIt & it, int & fd, EpollCreation & epoll, Handler & handler)
{
	int			fdFile;
	uint32_t	fileFdOpt;
	std::string client_request;
	
	(*it).ClientRequest();
	client_request = (*it).getClientRequest();
	std::string filePath;
	std::cout << "-->> \033[40mHandleClientEvent : request_size = " << client_request.size() << "\033[0m" << std::endl;
	if (client_request.size() == 0)
		throw std::runtime_error("Remove client");
	try
	{
		fdFile = (*it).processRequest(client_request, fileFdOpt, filePath);
		(*it).updateResponseCategory(STD_RSP);
	}
	catch(AExcept & e)
	{
		std::cerr << "Process Request exception catched : " << e._errMsg << std::endl;
		(*it).setCatchedResponse(e, fdFile, filePath);
		fileFdOpt = EPOLLIN;
		if ((*it).getCategoryResponse() == REDIR_RSP)
		{
			epoll.epoll_update_event(fd, EPOLLOUT);
			return;
		}
	}
	handler.addToVectFileFd(it, fdFile, fd, filePath);
	epoll.epoll_control(fileFdOpt, fdFile);
}

void	sendingResponseToClient(VClientIt & it, int & fd, EpollCreation & epoll)
{
	std::string	responseString;
	int			statusCode;
	int			fileStatus;
	int			responseCategory = (*it).getCategoryResponse();
	std::cout << "-->> \033[43mSendingResponseToClient : " << responseCategory << "\033[0m" << std::endl;

	if (responseCategory == REDIR_RSP)
	{
		responseString = (*it).getFullResponse();
		std::cout <<"\e[45mResponse: \n" << responseString << "\e[0m" << std::endl;
		send_value(fd, responseString);
		return ;
	}
	fileStatus = (*it).getFileStatus();
	if (responseCategory == STD_RSP)
		statusCode = 200;
	else
		statusCode = (*it).getStatusCodeResponse();
	if (fileStatus == OP_ONCE)
	{
		std::cout << "Sending standard response" << std::endl;
		(*it).setStandardResponse(statusCode);
	}
	else
	{
		std::cout << "Sending chunked response" << std::endl;
		(*it).setChunkedResponse(statusCode);
	}
	if (fileStatus == OP_ONGOING || fileStatus == OP_CLOSE)
		responseString = (*it).getBodyResponse() + "\r\n";
	else
		responseString = (*it).getFullResponse();
	// std::cout <<"\e[45mResponse: \n" << responseString << "\e[0m" << std::endl;
	send_value(fd, responseString);
	if (fileStatus != OP_ONGOING && fileStatus != OP_START)
		epoll.epoll_update_event(fd, EPOLLIN);
	if (fileStatus == OP_START)
		(*it).updateFileStatus(OP_ONGOING);
	if (fileStatus == OP_ONCE)
		delete (*it).getFileFD();
}

void	handleClientEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx)
{
	uint32_t	event = epoll.getEvent(eventIdx);
	VClientIt	it = handler.getClient(i);
	int			fd = (*it).get_clientFD();
	
	if ((event & EPOLLIN) && (*it).getConnexionStat() == true)
		manageRequestFromClient(it, fd, epoll, handler);
	else if ((event & EPOLLOUT) && (*it).getConnexionStat() == true)
		sendingResponseToClient(it, fd, epoll);
	else
	{
		epoll.epoll_clean(fd);
		handler.removeClient(i);
	}
}

void	readFileFd(vectFileFdIt & it, EpollCreation & epoll, Handler & handler, int const & i)
{
	std::cout << "\033[43mFileFd EPOLLIN : " << (*it)->getClientFd() << "\033[0m" << std::endl;
	int			fdClient;
	int			bytesRead;
	char		buffer[_BUFFER];
	std::string	bufferString;
	
	(*it)->manageFd();
	if ((*it)->getStatus() == OP_START)
		return;
	bytesRead = read((*it)->getFileFd(), buffer, _BUFFER);
	// std::cout << "BytesRead = " << bytesRead << std::endl;
	if (bytesRead <= 0)
	{
		(*it)->setStatus(OP_CLOSE);
		(*it)->clearBuffer();
		closeFileEvent(handler, i, epoll);
		return ;
	}
	if ((*it)->getStatus() == OP_OPEN && getFileSize((*it)->getFilePath()) <= static_cast<size_t>(bytesRead))
		(*it)->setStatus(OP_ONCE);
	bufferString.append(buffer, bytesRead);
	(*it)->setBuffer(bufferString);
	(*it)->restoreFd();
	fdClient = (*it)->getClientFd();
	// std::cout << "Fd = " << (*it)->getFileFd() << " from fd client = " << fdClient << std::endl;
	if ((*it)->getStatus() == OP_ONCE || (*it)->getStatus() == OP_OPEN)
	{
		if ((*it)->getStatus() == OP_OPEN)
			(*it)->setStatus(OP_START);
		epoll.epoll_update_event(fdClient, EPOLLOUT);
	}
	if ((*it)->getStatus() == OP_ONCE)
		closeFileEvent(handler, i, epoll);
}

void	handleFileEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx)
{
	vectFileFdIt	it = handler.getFileFd(i);
	uint32_t		event = epoll.getEvent(eventIdx);

	if (event & EPOLLIN) //Get request response
		readFileFd(it, epoll, handler, i);
	else if (event & EPOLLOUT)
	{
		std::cout << "\033[43mFD EPOLLOUT FOR FILE IO" << "\033[0m" << std::endl;
	}
	else
		std::cout << "ERROR ON EPOLL FILE IO\n";
}
