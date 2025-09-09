/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:29:39 by barandri          #+#    #+#             */
/*   Updated: 2025/09/03 09:42:29 by barandri         ###   ########.fr       */
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

void	initServer(vectServer & vectServer)
{
	std::for_each(vectServer.begin(), vectServer.end(), server_create);
}

void	launchEpoll(vectServer & vectServer, EpollCreation &epoll)
{
	int	flags;

	for(vectServerIt it = vectServer.begin(); it != vectServer.end(); it++)
	{
		flags = fcntl((*it).get_fdSocket(), O_NONBLOCK, 0);
		fcntl((*it).get_fdSocket(), F_SETFL, flags | O_NONBLOCK);
		int fd = ((*it).get_fdSocket());
		std::cout << "\033[046mSERVER FD= " << fd << "\033[0m" << std::endl;
		epoll.epoll_control(EPOLLIN, fd);
	}
}
