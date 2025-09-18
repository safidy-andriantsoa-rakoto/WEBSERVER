/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/18 14:24:49 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int sign = -1;

void handle_c(int SIGNUM)
{
	if (SIGNUM == SIGINT)
		sign = SIGINT;
}

void    launchServer(Handler & handler)
{
	int				fdEvent;
	int				index;
	int 			nb_event;
	std::string		identifier;
	EpollCreation	epoll;
	Signal			sign;

	initServer(handler.getVectServer());
	launchEpoll(handler.getVectServer(), epoll);
	while (SIGINT != sign.getSigNum())
	{
		nb_event = epoll.epoll_wait_event();
		index = 0;
		handler.displayHandlerStat();
		for (int i = 0; i < nb_event; i++)
		{
			try
			{
				fdEvent = epoll.get_fdEvent(i);
				std::cout << "\033[47;30m		FD EVENT = " << fdEvent << "\033[0m" << std::endl;
				identifier = handler.identifyFd(fdEvent, index);
				if (identifier == "SERVER")
				{
					std::cout << "\033[42m	SERVER" << "\033[0m" << std::endl;
					handleServerEvent(handler, index, epoll);
				}
				else if (identifier == "CLIENT")
				{
					std::cout << "\033[43m	CLIENT" << "\033[0m" << std::endl;
					handleClientEvent(handler, index, epoll, i);
				}
				else if (identifier == "FILEFD")
				{
					std::cout << "\033[43m	FILEFD" << "\033[0m" << std::endl;
					handleFileEvent(handler, index, epoll, i);
				}
			}
			catch(const std::exception& e)
			{
				std::cerr << "Error on main loop \n" << e.what() << '\n';
				if (identifier == "CLIENT")
				{
					epoll.epoll_clean(fdEvent);
					handler.removeClient(index);
				}
			}
		}
	}
}
