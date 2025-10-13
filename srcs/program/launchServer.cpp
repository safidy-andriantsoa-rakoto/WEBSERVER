/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/10/13 11:45:34 by rsafidy-         ###   ########.fr       */
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

int	readCgiStdError(vectCgiFdIt & itCgi, EpollCreation & epoll)
{
	int			bytesRead;
	char		buffer[_BUFFER];
	std::string	bufferString;

	bytesRead = read((*itCgi)->getErrFd(), buffer, _BUFFER);
	if (bytesRead <= 0)
	{
		// std::cout << "Nothing to read for CGI Err \n";
		epoll.epoll_clean((*itCgi)->getErrFd());
		return 0;
	}
	bufferString.append(buffer, bytesRead);
	std::cout << "				***************BUFFER ERROR \n" << bufferString << std::endl;
	return bytesRead;
}

void	handleCgiError(Handler & handler, int const & index, EpollCreation & epoll, AExcept & err, int const & identifier)
{
	int			fd[2] = {-1, -1};
	int			bytesRead = 0;
	vectCgiFdIt	itCgi = handler.getCgiFdIterator(index);
	VClientIt	it = findFdInClient(handler.getVectClient(), (*itCgi)->getClientFd());

	if (identifier == CGI_ERR_FD)
	{
		bytesRead = readCgiStdError(itCgi, epoll);
		if (!bytesRead)
			return;
	}
	(*it).setCatchedResponse(err, fd);
	if (fd[FD_STDOUT] == -1)
		return;
	epoll.epoll_clean((*itCgi)->getErrFd());
	epoll.epoll_clean((*itCgi)->getFd());
	handler.removeCgiFd(itCgi);
	if ((*it).getFdType() == HTML_FILE)
		handler.addToVectFileFd(it, fd[FD_STDOUT]);
	else
	{
		handler.addToVectCgiFd(it, fd[FD_STDOUT], fd[FD_STDERR]);
		epoll.epoll_control(EPOLLIN, fd[FD_STDERR]);
	}
	epoll.epoll_control(EPOLLIN, fd[FD_STDOUT]);
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
		handler.displayHandlerStat(); //Use for checking error
		for (int i = 0; i < nb_event; i++)
		{
			fdEvent = epoll.get_fdEvent(i);
			std::cout << "\033[47;30m		FD EVENT = " << fdEvent << "\033[0m" << std::endl;
			try
			{
				handler.identifyFd(fdEvent, index, identifier);
				callEvent(handler, epoll, index, identifier, i);
			}
			catch(std::exception & e)
			{
				std::cerr << "\033[41m" << fdEvent << " , idt = " << identifier << " => Error on epoll event loop : " << e.what() << "\033[0m" << std::endl;
				if (identifier == CLIENT)
				{
					handler.removeClient(index, epoll);
					epoll.epoll_clean(fdEvent);
				}
				if (identifier == CGI_FD || identifier == CGI_ERR_FD)
				{
					std::cerr << "CGI error identified" << std::endl;
					try
					{
						ErrorExcept	& err = dynamic_cast<ErrorExcept &>(e);
						handleCgiError(handler, index, epoll, err, identifier);
					}
					catch(const std::exception& e1)
					{
						std::cerr << "			Error on casting " << e1.what() << '\n';
					}
				}
				break ;
			}
			if (handler.getProgramBreak() == true) //Used to exit the child process correctly
				throw std::runtime_error("Program break launched");
			sleep(1);
		}
	}
}
