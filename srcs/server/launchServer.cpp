/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/29 15:23:57 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void run_process(int i, EpollCreation &epoll, VClient &v_client, vectServer & allServer)
{
	vectServerIt	itServ;
	VClientIt		itCli;
	int				fd;
	SEpollEvent		event;

	event = epoll.getEvent_struct(i);
	fd = epoll.get_fdEvent(i);
	if (event.events & EPOLLIN)
	{
		itServ = findFdInServer(allServer, fd);
		itCli = findFdInClient(v_client, fd);
		// std::cout << "\033[047mEVENT EPOLLIN NOW-->\033[0m" << event.data.fd << std::endl;
		if (itServ != allServer.end() && (*itServ).get_fdSocket() == fd)
		{
			int fdc = (*itServ).accept_client_connexion();
			std::cout << "\033[42mNEW CLIENT JOIN=\033[0m" << fdc << "-->\033[046mSERVER=" << fd << "\033[0m" << std::endl;
			epoll.epoll_control(EPOLLIN, fdc);
			Client client((*itServ).get_fdSocket(), fdc, (*itServ).get_confSockAddrIn(), false);
			v_client.push_back(client);
			epoll.epoll_update_event(fd, EPOLLIN);
		}
		else if (itCli != v_client.end() && (*itCli).get_clientFD() == fd)
		{
			(*itCli).ClientRequest();
			epoll.epoll_update_event(fd, EPOLLOUT);
			// std::cout
			// << "*******************************************\n" 
			// << (*itCli).getClientRequest()
			// << "*******************************************\n"
			// << std::endl;
		}
	}
	else if (event.events & EPOLLOUT)
	{
		// std::cout << "\033[047mEVENT EPOLLOUT NOW--->\033[0m" << event.data.fd << std::endl;
		itCli = findFdInClient(v_client, fd);
		itServ = findFdInServer(allServer, (*itCli).get_parent());
		std::cout << "\033[045mFD EVENT:" << fd << " | " << (*itCli).get_clientFD() << " WITH PARENT="<< (*itCli).get_parent() <<  "\033[0m" << std::endl;
		if (itCli != v_client.end())
		{
			// std::cout
			// << "************************2*******************\n" 
			// << (*itCli).getClientRequest()
			// << "**********************2********************\n"
			// << std::endl;
			std::string client_request;
			try
			{
				client_request = (*itCli).getClientRequest();
				// std::cout << "-->> \033[40m	" << client_request << "\033[0m" << std::endl;
				if (client_request.find("HTTP") != std::string::npos)
				{
					if (itServ != allServer.end())
					{
						(*itServ).processRequest(client_request, fd);
						std::cout << "ca marche" << std::endl;
					}
				}
			}
			catch(const std::exception& e)
			{
				std::cout << e.what();
				return ;
			}
		}
		epoll.epoll_update_event(fd, EPOLLIN);
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
	while (SIGINT != sign)
	{
		int nb_event = epoll.epoll_wait_event();
		if (nb_event)
		{
			for (int i = 0; i < nb_event; i++)
			{
				run_process(i, epoll, v_client, allServer);
				// sleep(1);
			}
		}
		// else
			// sleep(1);
	}
	for (size_t i = 0; i < v_client.size(); i++)
	{
		int fd = v_client[i].get_clientFD();
		std::cout << "end Client join fd =" << fd << std::endl;
		if (fd > -1)
			close(fd);
	}
	for (size_t i = 0; i < allServer.size(); i++)
	{
		int fd = allServer[i].get_fdSocket();
		if (fd > -1)
		{
			std::cout << "FD=" << fd << std::endl;
			close(fd);
		}
	}
}
