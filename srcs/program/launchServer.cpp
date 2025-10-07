/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/10/07 10:11:00 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int sign = -1;

void handle_c(int SIGNUM)
{
	if (SIGNUM == SIGINT)
		sign = SIGINT;
}

void	callEvent(Handler & handler, EpollCreation & epoll, int const & index, int const & identifier, int const & i)
{
	switch (identifier)
	{
		case SERVER:
			std::cout << "\033[46m	SERVER" << "\033[0m" << std::endl;
			handleServerEvent(handler, index, epoll);
			break;
		case CLIENT:
			std::cout << "\033[42m	CLIENT" << "\033[0m" << std::endl;
			handleClientEvent(handler, index, epoll, i);
			break;
		case FILE_FD:
			std::cout << "\033[43m	FILEFD" << "\033[0m" << std::endl;
			handleFileEvent(handler, index, epoll, i);
			break;
		case CGI_FD:
			std::cout << "\033[44m	CGIFD" << "\033[0m" << std::endl;
			handleCgiEvent(handler, index, epoll, i);
			break;
		default:
			throw std::runtime_error("Unknown fd");
			break;
	}
}

void	handleCgiError(Handler & handler, int const & index, EpollCreation & epoll, AExcept & err)
{
	int			fd = -1;
	vectCgiFdIt	itCgi = handler.getCgiFdIterator(index);
	VClientIt	it = findFdInClient(handler.getVectClient(), (*itCgi)->getClientFd());
	int			bytesRead;
	char		buffer[_BUFFER];
	std::string	bufferString;

	bytesRead = read((*itCgi)->getErrFd(), buffer, _BUFFER);
	if (bytesRead <= 0)
	{
		// std::cout << "Nothing to read for CGI Err \n";
		epoll.epoll_clean((*itCgi)->getErrFd());
		return;
	}
	bufferString.append(buffer, bytesRead);
	std::cout << "				***************BUFFER ERROR \n" << bufferString << std::endl;
	(*it).setCatchedResponse(err, fd);
	if (fd == -1)
		return;
	epoll.epoll_clean((*itCgi)->getErrFd());
	epoll.epoll_clean((*itCgi)->getFd());
	handler.removeCgiFd(itCgi);
	handler.addToVectFileFd(it, fd);
	epoll.epoll_control(EPOLLIN, fd);
}

void    launchServer(Handler & handler)
{
	int				fdEvent;
	int				index;
	int 			nb_event;
	int				identifier;
	Signal			sign;
	EpollCreation	epoll;

	initServer(handler.getVectServer());
	launchEpoll(handler.getVectServer(), epoll);
	while (SIGINT != sign.getSigNum())
	{
		nb_event = epoll.epoll_wait_event();
		handler.displayHandlerStat();
		for (int i = 0; i < nb_event; i++)
		{
			fdEvent = epoll.get_fdEvent(i);
			std::cout << "\033[47;30m		FD EVENT = " << fdEvent << "\033[0m" << std::endl;
			try
			{
				identifier = handler.identifyFd(fdEvent, index);
			}
			catch(AExcept & err)
			{
				std::cerr << "CGI error identified : " << err.what() << '\n';
				handleCgiError(handler, index, epoll, err);
				break;
			}
			try
			{
				callEvent(handler, epoll, index, identifier, i);
			}
			catch(const std::exception & e)
			{
				std::cerr << "\033[41mError on epoll event loop : " << e.what() << "\033[0m" << std::endl;
				std::string eWhat = e.what();
				if (eWhat == "AExcept error")
					throw std::runtime_error("Error page corrupted");
				if (identifier == CLIENT)
				{
					handler.removeClient(index, epoll);
					epoll.epoll_clean(fdEvent);
				}
				if (identifier == CGI_FD)
				{
					/*
						HandleCgiError()
						Create File_fd in error page through Client parent
						Add to epoll
						Clean class cgi_fd
					*/
				}
			}
			if (handler.getProgramBreak() == true)
			{
				std::cout << "				ENDING THE PROGRAM HERE !!!!" << std::endl;
				throw std::runtime_error("Program break launched");
			}
		}
	}
}
