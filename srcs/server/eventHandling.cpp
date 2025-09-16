/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/09/16 16:00:06 by barandri         ###   ########.fr       */
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

void	handleClientEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx)
{
	uint32_t	event = epoll.getEvent(eventIdx);
	VClientIt	it = handler.getClient(i);
	int			fd = (*it).get_clientFD();

	if ((event & EPOLLIN) && (*it).getConnexionStat() == true)
	{
		int			fdFile;
		uint32_t	opt;
		std::string client_request;
	
		(*it).ClientRequest();
		client_request = (*it).getClientRequest();
		std::string filePath;
		std::cout << "-->> \033[40m	" << "Request, size = " << client_request.size() << "\033[0m" << std::endl;
		if (client_request.size() == 0)
			throw std::runtime_error("Remove client");
		try
		{
			fdFile = (*it).processRequest(client_request, opt, filePath);
			(*it).updateResponseStatus(STD_RSP);
		}
		catch(AExcept & e)
		{
			std::cerr << "Process Request exception catched : " << e._errMsg << std::endl;
			(*it).setCatchedResponse(e);
			epoll.epoll_update_event(fd, EPOLLOUT);
			return;
		}
		handler.addToVectFileFd(fdFile, fd, filePath);
		epoll.epoll_control(opt, fdFile);
	}
	else if ((event & EPOLLOUT) && (*it).getConnexionStat() == true)
	{
		std::cout << "-->> \033[43mSending Response to client" << "\033[0m" << std::endl;
		std::string	responseString;
		int			statusCode;

		if ((*it).getResponse().getRespCategory() == STD_RSP)
		{
			statusCode = 200;
			if ((*it).getFileFD().getStatus() == OP_ONGOING)
			{
				std::cout << "Sending chunked response" << std::endl;
				(*it).setChunkedResponse(statusCode);
			}
			else if ((*it).getFileFD().getStatus() == OP_ONCE)
			{
				std::cout << "Sending standard response" << std::endl;
				(*it).setStandardResponse(statusCode);
			}
		}
		responseString = (*it).getResponse().getFullResponse();
		// if ((*it).getResponse().getRespCategory() != STD_RSP)
		// 	std::cout << "Sending redir or error response" << std::endl;
		if ((*it).getFileFD().getStatus() == OP_ONGOING)
		{
			std::cout << "Chunked \n" << responseString << std::endl;
		}
		send_value(fd, responseString);
		if ((*it).getFileFD().getStatus() != OP_ONGOING)
			epoll.epoll_update_event(fd, EPOLLIN); //Do not change if sending get_request is still ongoing
	}
	else
	{
		epoll.epoll_clean(fd);
		handler.removeClient(i);
	}
}

void	handleFileEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx)
{
	vectFileFdIt	it = handler.getFileFd(i);
	uint32_t		event = epoll.getEvent(eventIdx);

	if (event & EPOLLIN) //Get request response
	{
		std::cout << "\033[43mFD EPOLLIN FOR FILE IO" << "\033[0m" << std::endl;
		std::string	buffer;
		char		buf[_BUFFER];
		size_t		bytesRead;
		int			fd;
		int			fdClient;

		(*it).manageFd();
		bytesRead = read((*it).getFileFd(), buf, _BUFFER);
		if (bytesRead <= 0)
		{
			std::cout << "Nothing left to read from the file\n";
			closeFileEvent(handler, i, epoll);
			return ;
		}
		if (getFileSize((*it).getFilePath()) == bytesRead)
		{
			std::cout << "File to OP_ONCE" << std::endl;
			(*it).setStatus(OP_ONCE);
		}
		buffer.append(buf, bytesRead);
		(*it).setBuffer(buffer);
		(*it).restoreFd();
		fd = (*it).getFileFd();
		fdClient = (*it).getClientFd();
		std::cout << "Fd = " << fd  << "fd client = " << fdClient << std::endl;
		VClientIt itCli = findFdInClient(handler.getVectClient(), fdClient);
		(*itCli).setFileFD((*it));
		if ((*it).getStatus() != OP_CLOSE)
		{
			epoll.epoll_update_event(fdClient, EPOLLOUT);
			if ((*it).getStatus() == OP_OPEN)
				(*it).setStatus(OP_ONGOING);
		}
		if ((*it).getStatus() == OP_ONCE)
			closeFileEvent(handler, i, epoll);
	}
	else if (event & EPOLLOUT)
	{
		std::cout << "\033[43mFD EPOLLOUT FOR FILE IO" << "\033[0m" << std::endl;
	}
	else
		std::cout << "ERROR ON EPOLL FILE IO\n";
}
