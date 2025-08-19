/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/19 10:38:49 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void run_process(int i, EpollCreation &epoll, VClient &v_client, vectServer & allServer)
{
	vectServerIt	itServ;
	VClientIt		itCli;
	int				fd;

	fd = epoll.get_fdEvent(i);
	itServ = findFdInServer(allServer, fd);
	if (itServ != allServer.end()) //Add new client
	{
		int fdc = (*itServ).accept_client_connexion();
		epoll.epoll_control(EPOLLIN | EPOLLOUT, fdc);
		Client client((*itServ).get_fdSocket(), fdc, (*itServ).get_confSockAddrIn(), false);
		v_client.push_back(client);
	}
	else //Run process request
	{
		itCli = findFdInClient(v_client, fd);
		itServ = findFdInServer(allServer, (*itCli).get_parent());
		(*itServ).processRequest((*itCli).ClientRequest(), fd);
		epoll.epoll_clean(fd);
		close(fd);
		v_client.erase(itCli);
	}
}

void    launchServer(vectServer & allServer)
{
	EpollCreation	epoll;
	VClient			v_client;

	initAllServer(allServer);
	launchEpoll(allServer, epoll);
	while (1)
	{
		for (int i = 0; i < epoll.epoll_wait_event(); i++)
			run_process(i, epoll, v_client, allServer);
	}
}
