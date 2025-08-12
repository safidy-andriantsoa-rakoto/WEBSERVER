/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/12 14:57:41 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void    launchServer(vectServer & allServer)
{
	vectServerIt		it;
	vectServerIt		it2;
	std::string			request;
	EpollCreation		epoll;
	std::vector<Client>	v_client;

	initAllServer(allServer);
	launchEpoll(allServer, epoll);
	while (1)
	{
		for (int i = 0; i < epoll.epoll_wait_event(); i++)
		{
			int fd = epoll.get_fdEvent(i);
			std::cout << "	Server fd  = " << fd << std::endl;
			it = findFd(allServer, fd);//socket
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
		sleep(1);
	}
}

