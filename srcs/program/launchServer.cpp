/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/10/01 14:26:29 by rsafidy-         ###   ########.fr       */
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
			std::cout << "\n\n\e[5m		INDEX-->" << i << "\e[0m\n" << std::endl;
			try
			{
				fdEvent = epoll.get_fdEvent(i);
				std::cout << "\033[47;30m		FD EVENT = " << fdEvent << "\033[0m" << std::endl;
				identifier = handler.identifyFd(fdEvent, index);
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
					default:
						throw std::runtime_error("Unknown fd");
						break;
				}
			}
			catch(const std::exception& e)
			{
				std::cerr << "\e[41mError on main loop : " << e.what() << "\e[0m" << std::endl;
				std::string eWhat = e.what();
				if (eWhat == "AExcept error")
					throw std::runtime_error("Error page corrupted");
				if (identifier == CLIENT)
				{
					handler.removeClient(index, epoll);
					epoll.epoll_clean(fdEvent);
				}
			}
			// sleep(2);
		}
	}
}
