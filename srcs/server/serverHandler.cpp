/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/08/27 14:21:06 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void server_create(Server & server)
{
	if (!server.create_socket())
		throw std::runtime_error("Failed to load server");
	server.configure_sockaddrin(server.getConfig().getServPort());
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
	int	flags;

	for(vectServerIt it = allServer.begin(); it != allServer.end(); it++)
	{
		flags = fcntl((*it).get_fdSocket(), O_NONBLOCK, 0);
		fcntl((*it).get_fdSocket(), F_SETFL, flags | O_NONBLOCK);
s		int fd = ((*it).get_fdSocket());
		std::cout << "\033[046mSERVER FD= " << fd << "\033[0m" << std::endl;
		epoll.epoll_control(EPOLLIN, fd);
	}
}

vectServerIt	findFdInServer(vectServer & allServer, int const & fd)
{
	vectServerIt it;

	for(it = allServer.begin(); it != allServer.end(); it++)
		if ((*it).get_fdSocket() == fd)
			break;
	return it;
}