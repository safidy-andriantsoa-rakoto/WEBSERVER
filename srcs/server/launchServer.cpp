/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/27 14:26:33 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void run_process(int i, EpollCreation &epoll, VClient &v_client, vectServer & allServer)
{
	vectServerIt	itServ;
	VClientIt		itCli;
	int				fd;

	fd = epoll.get_fdEvent(i);
	std::cout << "		FD EVENT:" << fd << std::endl;
	itServ = findFdInServer(allServer, fd);
	if (itServ != allServer.end())
	{
		int fdc = (*itServ).accept_client_connexion();
		std::cout << "\033[42mNEW CLIENT JOIN=\033[0m" << fdc << "-->\033[046mSERVER=" << fd << "\033[0m" << std::endl;
		epoll.epoll_control(EPOLLIN | EPOLLOUT, fdc);
		Client client((*itServ).get_fdSocket(), fdc, (*itServ).get_confSockAddrIn(), false);
		v_client.push_back(client);
	}
	else //Run process request
	{
			itCli = findFdInClient(v_client, fd);
			std::cout << "\033[45mFD EVENT:" << fd << " | " << (*itCli).get_clientFD() << "\033[0m" << std::endl;
			itServ = findFdInServer(allServer, (*itCli).get_parent());
			if (itServ != allServer.end())
			{
				std::string client_request;
				try
				{
					client_request = (*itCli).ClientRequest();
					// std::cout << "-->> \033[40m	" << client_request << "\033[0m" << std::endl;
					(*itServ).processRequest(client_request, fd);
				}
				catch(const std::exception& e)
				{
					std::cout << e.what();
					return ;
				}
				// epoll.epoll_clean(fd);
				// close(fd);
			}
	}
	// else
	// {
	// 	for (size_t i = 0; i < v_client.size(); i++)
	// 	{
	// 		int fd_rec = v_client[i].get_clientFD();
	// 		std::cout << "\033[41mCLIENT FD=\033[0m " << fd_rec << " et de i = " << i << std::endl;
	// 		if (fd == fd_rec)
	// 		{
	// 			it2 = findFd(allServer, v_client[i].get_parent());
	// 			(*it2).processRequest(v_client[i].ClientRequest(), fd);
	// 			epoll.epoll_clean(fd);
	// 		}
	// 	}
	// 	// close(fd);
	// 	// v_client.pop_back();
	// }
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
				sleep(1);
			}
		}
		sleep(1);
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
