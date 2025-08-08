/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/08 11:57:20 by rsafidy-         ###   ########.fr       */
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

void    launchServer(vectServer & allServer)
{
	itVectServer		it;
	itVectServer		it2;
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
				//miandry client vaovao ra socket fd no azo
				std::cout << "aty eefd =" << (*it).get_fdSocket() << std::endl;
				int fdc = (*it).accept_client_connexion();
				std::cout << "vita fdc =" << fdc << std::endl;
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

