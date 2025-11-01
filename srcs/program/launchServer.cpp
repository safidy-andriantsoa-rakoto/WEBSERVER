/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/10/31 08:31:21 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int sign = -1;

void handle_c(int SIGNUM)
{
	if (SIGNUM == SIGINT)
		sign = SIGINT;
}

void	manageEpollLoopError(Handler & handler, EpollCreation & epoll, int const & identifier, int const & fdEvent, int const & index, std::exception & e)
{
	if (identifier == CLIENT)
	{
		std::string error = e.what();
		if (error == "Execve error")
			throw std::runtime_error(e.what());
		handler.removeClient(index, epoll);
		epoll.epoll_clean(fdEvent);
	}
	if (identifier == CGI_FD_OUTPUT || identifier == CGI_FD_INPUT)
	{
		std::cerr << "CGI error identified" << std::endl;
		try
		{
			ErrorExcept	& err = dynamic_cast<ErrorExcept &>(e);
			handleCgiError(handler, index, epoll, err);
		}
		catch(const std::exception& e1)
		{
			std::cerr << "			Error on CGI exception " << e1.what() << '\n';
			// closeCgiFdEvent(handler, index, epoll);
		}
	}
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
		case CGI_FD_OUTPUT:
			std::cout << "\033[44m	CGIFD OUTPUT" << "\033[0m" << std::endl;
			readCgiOutput(handler, index, epoll);
			break;
		case CGI_FD_INPUT:
			std::cout << "\033[44m	CGIFD INPUT" << "\033[0m" << std::endl;
			writeCgiInput(handler, index, epoll);
			break;
		default:
			throw std::runtime_error("Unknown fd");
			break;
	}
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
		nb_event = epoll.epoll_wait_event(); //No need for try catch
		handler.displayHandlerStat(); /* [Test only] */
		handler.check_client_timeout_all(epoll);
		for (int i = 0; i < nb_event; i++)
		{
			fdEvent = epoll.get_fdEvent(i);
			std::cout << "\033[47;30m		FD EVENT = " << fdEvent << " : " << handler.getIsChildProcess() << "\033[0m" << std::endl;
			try
			{
				handler.identifyFd(fdEvent, index, identifier);
				callEvent(handler, epoll, index, identifier, i);
			}
			catch(std::exception & e)
			{
				std::cerr << "\033[41m" << fdEvent << " , idt = " << getIdentifier(identifier) << " => Error on epoll event loop : " << e.what() << "\033[0m" << std::endl;
				manageEpollLoopError(handler, epoll, identifier, fdEvent, index, e);
				break ;
			}
			if (handler.getProgramBreak() == true)
				throw std::runtime_error("Program break launched");
		}
	}
}
