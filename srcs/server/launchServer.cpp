/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/09 12:45:14 by barandri         ###   ########.fr       */
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

	initServer(handler.getVectServer());
	launchEpoll(handler.getVectServer(), epoll);
	signal(SIGINT, handle_c);
	while (SIGINT != sign)
	{
		nb_event = epoll.epoll_wait_event();
		index = 0;
		handler.displayHandlerStat();
		for (int i = 0; i < nb_event; i++)
		{
			fdEvent = epoll.get_fdEvent(i);
			std::cout << "FD EVENT = " << fdEvent << std::endl;
			identifier = handler.identifyFd(fdEvent, index);
			if (identifier == "SERVER")
				handleClient(handler, index, epoll);
			else if (identifier == "CLIENT")
				handleRequest(handler, index, epoll, i);
			else if (identifier == "FILEFD")
				handleFileIO(handler, index, epoll, i);
			else
				std::cout << "Unknown Fd !!!!" << std::endl;
		}
	}
}
