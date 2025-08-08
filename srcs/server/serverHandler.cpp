/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/08/07 14:51:44 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void server_create(Server & server)
{
	if (!server.create_socket())
		throw std::runtime_error("Failed to load server");
	server.configure_sockaddrin(server.getPort());
	if (!server.link_socket_adress())
		throw std::runtime_error("Failed to load server");
	if (!server.wait_connexion_from_client())
		throw std::runtime_error("Failed to load server");

}

void	initAllServer(vectServer & allServer)
{
	std::for_each(allServer.begin(), allServer.end(), server_create);
}

int	launchEpoll(vectServer & allServer)
{
	int					efds;
	int					flags;
	struct epoll_event	event;

	efds = epoll_create1(0);
	if (efds < 0)
		throw std::runtime_error("Error on poll create");
	for(itVectServer it = allServer.begin(); it != allServer.end(); it++)
	{
		flags = fcntl((*it).get_fdSocket(), O_NONBLOCK, 0);
		fcntl((*it).get_fdSocket(), F_SETFL, flags | O_NONBLOCK);
		event.events = EPOLLIN | EPOLLOUT;
		event.data.fd = (*it).get_fdSocket();
		if(epoll_ctl(efds, EPOLL_CTL_ADD, (*it).get_fdSocket(), &event) < 0)
			throw std::runtime_error("epoll_ctl_add error");
	}
	return efds;
}

itVectServer	findFd(vectServer & allServer, int const & fd)
{
	itVectServer it;

	for(it = allServer.begin(); it != allServer.end(); it++)
	{
		if ((*it).get_fdSocket() == fd)
			break;
	}
	return it;
}