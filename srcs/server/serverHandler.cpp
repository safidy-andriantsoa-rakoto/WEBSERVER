/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/08/08 10:59:36 by rsafidy-         ###   ########.fr       */
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

void	launchEpoll(vectServer & allServer, EpollCreation &epoll)
{
	int					flags;

	for(itVectServer it = allServer.begin(); it != allServer.end(); it++)
	{
		flags = fcntl((*it).get_fdSocket(), O_NONBLOCK, 0);
		fcntl((*it).get_fdSocket(), F_SETFL, flags | O_NONBLOCK);
		int fd = ((*it).get_fdSocket());
		epoll.epoll_control(EPOLLIN | EPOLLOUT, fd);
	}
}

itVectServer	findFd(vectServer & allServer, int const & fd)
{
	itVectServer it;

	for(it = allServer.begin(); it != allServer.end(); it++)
	{
		std::cout << "SOCKET=" << (*it).get_fdSocket() << std::endl;
		if ((*it).get_fdSocket() == fd)
			break;
	}
	return it;
}