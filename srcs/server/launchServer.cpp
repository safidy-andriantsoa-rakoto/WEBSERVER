/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/28 15:38:37 by rsafidy-         ###   ########.fr       */
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
	if (event.events & EPOLLRDHUP)
		std::cout << "\033[031,047mDECONECTION AU NIVEAU DU CLIENT\033[0m" << std::endl;
	if (event.events & EPOLLIN)
		std::cout << "\033[047mEVENT EPOLLIN NOW-->\033[0m" << event.data.fd << std::endl;
	if (event.events & EPOLLOUT)
		std::cout << "\033[047mEVENT EPOLLOUT NOW--->\033[0m" << event.data.fd << std::endl;
	// std::cout << "		FD EVENT:" << fd << std::endl;
	if (event.events & EPOLLIN)
	{
		itServ = findFdInServer(allServer, fd);
		itCli = findFdInClient(v_client, fd);
		if (itServ != allServer.end() && (*itServ).get_fdSocket() == fd)
		{
			int fdc = (*itServ).accept_client_connexion();
			std::cout << "\033[42mNEW CLIENT JOIN=\033[0m" << fdc << "-->\033[046mSERVER=" << fd << "\033[0m" << std::endl;
			epoll.epoll_control(EPOLLIN | EPOLLOUT, fdc);
			Client client((*itServ).get_fdSocket(), fdc, (*itServ).get_confSockAddrIn(), false);
			v_client.push_back(client);
		}
		else if (itCli != v_client.end() && (*itCli).get_clientFD() == fd)
		{
			std::string		client_request;
			client_request = (*itCli).ClientRequest();
			std::cout
			<< "*******************************************\n" 
			<< client_request
			<< "*******************************************\n"
			<< std::endl;
			// epoll.epoll_control();
		}
	}
	if (event.events & EPOLLOUT)
	{
		itCli = findFdInClient(v_client, fd);
		std::cout << "\033[045mFD EVENT:" << fd << " | " << (*itCli).get_clientFD() << "\033[0m" << std::endl;
		itServ = findFdInServer(allServer, (*itCli).get_parent());
		if (itCli != v_client.end())
		{
			std::string client_request;
			try
			{
				// client_request = (*itCli).ClientRequest();
				// std::cout << "-->> \033[40m	" << client_request << "\033[0m" << std::endl;
				if (itServ != allServer.end())
					(*itServ).processRequest((*itCli).getClientRequest(), fd);
			}
			catch(const std::exception& e)
			{
				std::cout << e.what();
				return ;
			}
		}
		else
			std::cout << "		ERROR, client fd" << std::endl;
	}
	// if (itServ != allServer.end())
	// {
	// 	int fdc = (*itServ).accept_client_connexion();
	// 	std::cout << "\033[42mNEW CLIENT JOIN=\033[0m" << fdc << "-->\033[046mSERVER=" << fd << "\033[0m" << std::endl;
	// 	epoll.epoll_control(EPOLLIN | EPOLLOUT, fdc);
	// 	Client client((*itServ).get_fdSocket(), fdc, (*itServ).get_confSockAddrIn(), false);
	// 	v_client.push_back(client);
	// }
	// else //Run process request
	// {
	// 		itCli = findFdInClient(v_client, fd);
	// 		std::cout << "\033[45mFD EVENT:" << fd << " | " << (*itCli).get_clientFD() << "\033[0m" << std::endl;
	// 		itServ = findFdInServer(allServer, (*itCli).get_parent());
	// 		if (itCli != v_client.end())
	// 		{
	// 			std::string client_request;
	// 			try
	// 			{
	// 				client_request = (*itCli).ClientRequest();
	// 				// std::cout << "-->> \033[40m	" << client_request << "\033[0m" << std::endl;
	// 				if (itServ != allServer.end())
	// 					(*itServ).processRequest(client_request, fd);
	// 			}
	// 			catch(const std::exception& e)
	// 			{
	// 				std::cout << e.what();
	// 				return ;
	// 			}
	// 		}
	// 		else
	// 			std::cout << "		ERROR, client fd" << std::endl;
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
