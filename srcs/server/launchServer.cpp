/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/12 15:07:21 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


void    launchServer(vectServer & allServer)
{
	itVectServer		it;
	itVectServer		it2;
	int					fd;

	fd = epoll.get_fdEvent(i);
	it = findFd(allServer, fd);
	if (it != allServer.end())
	{
		int fdc = (*it).accept_client_connexion();
		epoll.epoll_control(EPOLLIN | EPOLLOUT, fdc);
		Client client((*it).get_fdSocket(), fdc, (*it).get_confSockAddrIn(), false);
		v_client.push_back(client);
	}
	else
	{
		it2 = findFd(allServer, v_client.begin()->get_parent());
		(*it2).processRequest(v_client.begin()->ClientRequest(), fd);
		epoll.epoll_clean(fd);
		close(fd);
		v_client.pop_back();
	}
}

void    launchServer(vectServer & allServer)
{
	std::string			request;
	EpollCreation		epoll;
	VClient	v_client;

	initAllServer(allServer);
	launchEpoll(allServer, epoll);
	while (1)
	{
		for (int i = 0; i < epoll.epoll_wait_event(); i++)
			run_process(i, epoll, v_client, allServer);
	}
}
