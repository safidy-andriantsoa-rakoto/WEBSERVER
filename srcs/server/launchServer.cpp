/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/14 15:05:20 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// static std::string get_client_request(int fdCli)
// {
//     char response[_BUFFER];
//     int _respStatus = recv(fdCli, response, sizeof(response), 0);
//     if (_respStatus == -1)
//     {
//         std::cout << "Error\nreceive a message from a socket" << std::endl;
//         return ("");
//     }
//     return (response);
// }

// static std::string	establishConnectionToClient(Server & server, int & fd_cli)
// {
// 	size_t				size;
// 	std::string			request;
// 	struct sockaddr_in	cli;

// 	cli.sin_family = AF_INET;
// 	cli.sin_port = htons(server.getPort());
// 	cli.sin_addr.s_addr = INADDR_ANY;
// 	size = sizeof(cli);
// 	fd_cli = accept(server.get_fdSocket(), (sockaddr *)&(cli), (socklen_t *)&(size));
// 	if (fd_cli < 0)
// 		throw std::runtime_error("Accept error");
// 	request = get_client_request(fd_cli);
// 	return request;
// }

static void run_process(int i, EpollCreation &epoll, VClient &v_client, vectServer & allServer)
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

int sign = -1;

void handle_c(int SIGNUM)
{
	if (SIGNUM == SIGINT)
		sign = SIGINT;
}

void    launchServer(vectServer & allServer)
{
	std::string			request;
	EpollCreation		epoll;
	VClient	v_client;

	initAllServer(allServer);
	launchEpoll(allServer, epoll);
	signal(SIGINT, handle_c);
	while (SIGINT != sign)
	{
		for (int i = 0; i < epoll.epoll_wait_event(); i++)
			run_process(i, epoll, v_client, allServer);
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
