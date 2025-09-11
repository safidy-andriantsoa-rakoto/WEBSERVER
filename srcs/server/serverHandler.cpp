/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/09/11 14:47:47 by barandri         ###   ########.fr       */
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
	VClientIt	it = handler.getClient(i);
	int			fd = (*it).get_clientFD();
	uint32_t	event = epoll.getEvent(eventIdx);

	if ((event & EPOLLIN) && (*it).getConnexionStat() == true)
	{
		int			fdFile;
		int			fdServer = (*it).get_fdparent();
		uint32_t	opt;
		std::string client_request;
		Server	server = *(findFdInServer(handler.getVectServer(), fdServer));
	
		(*it).ClientRequest();
		client_request = (*it).getClientRequest();
		std::string filePath;
		std::cout << "-->> \033[40m	" << "Request, size = " << client_request.size() << "\033[0m" << std::endl;
		if (client_request.size() == 0)
		{
			throw std::runtime_error("Remove client");
			// return ;
		}
		try
		{
			fdFile = server.processRequest(client_request, opt, filePath);
		}
		catch(AExcept & e)
		{
			t_errPage	errPage;
			std::cerr << "Process Request exception catched : " << e._errMsg << std::endl; //Fafana
			errPage = server.getConfig().getErrPage();
			(*it).setCatchedResponse(e, errPage);
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

		if ((*it).getResponse().getRespCategory() == STD_RSP)
		{
			(*it).setStandardResponse(200);
			// std::cout << "Sending standard response" << std::endl;
		}
		responseString = (*it).getResponse().getFullResponse();
		// if (client.getResponse().getRespCategory() != STD_RSP)
		// 	std::cout << "Sending redir or error response" << std::endl;
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
		VClientIt itCli = findFdInClient(handler.getVectClient(), fdClient);
		epoll.epoll_update_event(fdClient, EPOLLOUT);
		(*itCli).setFileFD((*it));
		/**************************** */
		epoll.epoll_clean(fd);
		(*it).closeAllOwnedFileFd();
		handler.removeFileFd(i);
		// std::cout << "HANDLE FILE IO DONE \n";
	}
	else if (event & EPOLLOUT)
	{
		std::cout << "\033[43mFD EPOLLOUT FOR FILE IO" << "\033[0m" << std::endl;
	}
	else
		std::cout << "ERROR ON EPOLL FILE IO\n";
}
