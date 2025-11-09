/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:29:39 by barandri          #+#    #+#             */
/*   Updated: 2025/11/09 11:07:25 by barandri         ###   ########.fr       */
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
		int port = (*it).getConfig().getServPort()[0];
		if (!(*it).resolve_host((*it).getConfig().getServHost(), port)) //hostname
			throw std::runtime_error("Failed to load server");
		if (!(*it).link_socket_adress())
		{
			std::cout << "\033[31mFAILED TO BIND ON IP: ... \033[0m" << std::endl;
			std::cout << "\033[32mRedirecting to: default(0.0.0.0)\033[0m" << std::endl;
			(*it).getConfig().setHostName("0.0.0.0");
			(*it).configure_sockaddrin(port);
			if ((*it).link_socket_adress() == 0)
				throw std::runtime_error("Failed to load server");
		}
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
