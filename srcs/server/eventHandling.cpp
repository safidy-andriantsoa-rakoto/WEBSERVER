/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/09/22 11:41:45 by rsafidy-         ###   ########.fr       */
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

	if ((event & EPOLLIN) && (*it).getConnexionStat() == true)// verification si le client reste en EPOLLIN
	{
		int			fdFile;
		uint32_t	opt;
		std::string client_request;
		std::string limit = "\r\n\r\n";
		std::string body;
		std::string filePath;
		Request		tmpRequest;
		std::string limiter = "--";
		
		tmpRequest = (*it).getobjREquest();
		(*it).ClientRequest();
		client_request = (*it).getClientRequest();
		if (client_request.size() == 0)
			throw std::runtime_error("Remove client");
		try
		{
			size_t pos = client_request.find(limit);
			if (pos != std::string::npos)
			{
				std::string header = client_request.substr(0, pos);
				std::cout << "HEADER====>" << header << "]" << std::endl;
				fdFile = (*it).processRequest(client_request, opt, filePath);
				body = client_request.substr(pos + limit.size(), client_request.size());
				if ((*it).getBoundaryLimit().size() && body.size())
				{
					limiter += (*it).getBoundaryLimit();
					size_t is_boundary = body.find(limiter);
					body.erase(0, is_boundary);// deletena ny any aloha
					while (is_boundary != std::string::npos)
					{
						// std::cout << "POS LIMITER->" << is_boundary << std::endl;
						body.erase(0, is_boundary + limiter.size());
						is_boundary = body.find(limiter);
						if (is_boundary != std::string::npos)
						{
							std::string tmp = body.substr(0, is_boundary);
							std::cout << "BODY=====>[" << tmp << "]" << std::endl;
						}
						else
						{
							std::cout << "BODY==END===>[" << body << "]" << std::endl;
						}
					}
				}
				(*it).updateResponseStatus(STD_RSP);
			}
			else
				std::cout << "RIEN A TROOUVE! OOPS" << std::endl;
		}
		catch(AExcept & e)
		{
			std::cerr << "Process Request exception catched : " << e._errMsg << std::endl;
			(*it).setCatchedResponse(e);
			epoll.epoll_control(opt, fdFile);
			// epoll.epoll_update_event(fd, EPOLLOUT);
			return;
		}
		handler.addToVectFileFd(fdFile, fd, filePath);
		epoll.epoll_control(opt, fdFile);
	}
	else if ((event & EPOLLOUT) && (*it).getConnexionStat() == true)
	{
		std::cout << "-->> \033[44mSending Response to client" << "\033[0m" << std::endl;
		std::string	responseString;

		if ((*it).getResponse().getRespCategory() == STD_RSP)
		{
			(*it).setStandardResponse(200);
			std::cout << "Sending standard response" << std::endl;
		}
		responseString = (*it).getResponse().getFullResponse();
		if ((*it).getResponse().getRespCategory() != STD_RSP)
			std::cout << "Sending redir or error response" << std::endl;
		send_value(fd, responseString);
		epoll.epoll_update_event(fd, EPOLLIN);
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
		char buf[_BUFFER];
		std::string buffer;
		size_t bytesRead;
		int fd;
		int	fdClient;

		(*it).manageFd();
		while ((bytesRead = read((*it).getFileFd(), buf, _BUFFER)) > 0)
			buffer.append(buf, bytesRead);
		(*it).setBuffer(buffer);
		(*it).restoreFd();
		fd = (*it).getFileFd();
		std::cout << "Fd = " << fd << std::endl;
		/*Change client to EPOLLOUT*/
		fdClient = (*it).getClientFd();
		epoll.epoll_update_event(fdClient, EPOLLOUT);
		VClientIt itCli = findFdInClient(handler.getVectClient(), fdClient);
		(*itCli).setFileFD((*it));
		/**************************** */
		epoll.epoll_clean(fd);
		close((*it).getStockFd());
		handler.removeFileFd(i);
	}
	else if (event & EPOLLOUT)
	{
		std::cout << "ATY QQQQQQQQQQQQQQQQQQQQQQQQ		\033[43mFD EPOLLOUT FOR FILE IO" << "\033[0m" << std::endl;
	}
	else
		std::cout << "ERROR ON EPOLL FILE IO\n";
}
