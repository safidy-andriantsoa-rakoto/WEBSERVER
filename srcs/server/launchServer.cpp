/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/08 15:21:48 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void run_process(int i, EpollCreation &epoll, VClient &v_client, vectServer & allServer)
{
	// std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
	vectServerIt	itServ;
	VClientIt		itCli;
	int				fd;
	SEpollEvent		event;

	event = epoll.getEvent_struct(i);
	fd = event.data.fd;
	itServ = findFdInServer(allServer, fd);
	if (itServ != allServer.end())
	{
		int fdc;

		fdc = (*itServ).accept_client_connexion();
		while (fdc > 0)
		{
			epoll.no_blocking_fd(fdc);
			epoll.epoll_control(EPOLLIN, fdc);
			Client client(
				(*itServ).get_fdSocket(),
				fdc,
				(*itServ).get_confSockAddrIn(),
				true
			);
			v_client.push_back(client);
			fdc = (*itServ).accept_client_connexion();
		}
		// epoll.epoll_update_event(fd, EPOLLIN);
	}
	else
	{
		if (event.events & EPOLLIN) // mizara roa ra client de ->request ilay izy, ra FD file -> read()
		{
			std::cout << "______________________________________" << std::endl;
			itCli = findFdInClient(v_client, fd);
			if (itCli != v_client.end())
			{
				if ((*itCli).getConnexionStat())
				{
					try
					{
						(*itCli).ClientRequest();
						epoll.epoll_update_event(fd, EPOLLOUT);
					}
					catch(const std::exception& e)
					{
						epoll.epoll_clean(fd);
						v_client.erase(itCli);
						std::cerr << e.what() << '\n';
					}
				}
				else
				{
					epoll.epoll_clean(fd);
					v_client.erase(itCli);
				}
			}
		}
		else if (event.events & EPOLLOUT)
		{
			itCli = findFdInClient(v_client, fd);
			itServ = findFdInServer(allServer, (*itCli).get_parent());
			if (itCli != v_client.end())
			{
				if ((*itCli).getConnexionStat())
				{
					std::string client_request;
					try
					{
						client_request = (*itCli).getClientRequest();
						if (client_request.find("HTTP") != std::string::npos)
						{
							if (itServ != allServer.end())
								(*itServ).processRequest(client_request, fd);
						}
						client_request.clear();
						epoll.epoll_update_event(fd, EPOLLIN);
					}
					catch(const std::exception& e)
					{
						epoll.epoll_update_event(fd, EPOLLIN);
						std::cout << e.what();
						return ;
					}
				}
				else
				{
					epoll.epoll_clean(fd);
					v_client.erase(itCli);
				}
			}
		}	
	}
}

int sign = -1;

void handle_c(int SIGNUM)
{
	if (SIGNUM == SIGINT)
		sign = SIGINT;
}

void    launchServer(vectServer & allServer)
{
	EpollCreation	epoll;
	VClient			v_client;

	initAllServer(allServer);
	launchEpoll(allServer, epoll);
	signal(SIGINT, handle_c);
	signal(SIGPIPE, SIG_IGN); // pourque le send ne crash pas
	while (SIGINT != sign)
	{
		int nb_event = epoll.epoll_wait_event();
		if (nb_event)
		{
			for (int i = 0; i < nb_event; i++)
			{
				std::cout << "["<< i <<"]" << "EVENTS------------------"<< nb_event << std::endl;
				run_process(i, epoll, v_client, allServer);
			}
		}
	}
	for (size_t i = 0; i < v_client.size(); i++)
	{
		int fd = v_client[i].get_clientFD();
		epoll.epoll_clean(fd);
		std::cout << "\nclose Client join fd =" << fd << std::endl;
	}
	for (size_t i = 0; i < allServer.size(); i++)
	{
		int fd = allServer[i].get_fdSocket();
		epoll.epoll_clean(fd);
	}
}
