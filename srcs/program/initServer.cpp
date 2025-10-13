/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:29:39 by barandri          #+#    #+#             */
/*   Updated: 2025/10/02 13:39:48 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void server_create(Server & server)
{
	if (!server.create_socket())
		throw std::runtime_error("Failed to load server");
}

void	initServer(vectServer & vectServer)
{
	std::for_each(vectServer.begin(), vectServer.end(), server_create);
}

void	launchEpoll(vectServer & vectServer, EpollCreation &epoll)
{
	for(vectServerIt it = vectServer.begin(); it != vectServer.end(); it++)
	{
		int fd = ((*it).get_fdSocket());
		std::cout << "\033[046mSERVER FD= " << fd << "\033[0m" << std::endl;
		epoll.epoll_control(EPOLLIN, fd);
		(*it).configure_sockaddrin((*it).getConfig().getServPort());
		if (!(*it).link_socket_adress())
			throw std::runtime_error("Failed to load server");
		if (!(*it).wait_connexion_from_client())
			throw std::runtime_error("Failed to load server");
	}
}

std::string currentDateTimeString()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[15];

    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tstruct);
    return (buf);
}