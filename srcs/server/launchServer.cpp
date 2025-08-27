/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/27 11:53:38 by rsafidy-         ###   ########.fr       */
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
	std::cout << "		FD EVENT:" << fd << std::endl;
	it = findFd(allServer, fd);

	if (it != allServer.end())
	{
		int fdc = (*it).accept_client_connexion();
		std::cout << "\033[42mNEW CLIENT JOIN=\033[0m" << fdc << "-->\033[046mSERVER=" << fd << "\033[0m" << std::endl;
		epoll.epoll_control(EPOLLIN | EPOLLOUT, fdc);
		Client client((*it).get_fdSocket(), fdc, (*it).get_confSockAddrIn(), false);
		v_client.push_back(client);
	}
	else
	{
		for (size_t i = 0; i < v_client.size(); i++)
		{
			std::cout << "\033[45mFD EVENT:" << fd << " | " << v_client[i].get_clientFD() << "\033[0m" << std::endl;
			if (v_client[i].get_clientFD() == fd)
			{
				it2 = findFd(allServer, v_client[i].get_parent());
				if (it2 != allServer.end())
				{
					std::string client_request;
					try
					{
						client_request = v_client[i].ClientRequest();
						// std::cout << "-->> \033[40m	" << client_request << "\033[0m" << std::endl;
						(*it2).processRequest(client_request, fd);
					}
					catch(const std::exception& e)
					{
						std::cout << e.what();
						return ;
					}
					// epoll.epoll_clean(fd);
					// close(fd);
				}
			}
		}
	}
	// else
	// {
	// 	for (size_t i = 0; i < v_client.size(); i++)
	// 	{
	// 		int fd_rec = v_client[i].get_clientFD();
	// 		std::cout << "\033[41mCLIENT FD=\033[0m " << fd_rec << " et de i = " << i << std::endl;
	// 		if (fd == fd_rec)
	// 		{
	// 			it2 = findFd(allServer, v_client[i].get_parent());
	// 			(*it2).processRequest(v_client[i].ClientRequest(), fd);
	// 			epoll.epoll_clean(fd);
	// 		}
	// 	}
	// 	// close(fd);
	// 	// v_client.pop_back();
	// }
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
		int nb_event = epoll.epoll_wait_event();
		if (nb_event)
		{
			for (int i = 0; i < nb_event; i++)
			{
				run_process(i, epoll, v_client, allServer);
				sleep(1);
			}
		}
		sleep(1);
	}
	for (size_t i = 0; i < v_client.size(); i++)
	{
		int fd = v_client[i].get_clientFD();
		std::cout << "end Client join fd =" << fd << std::endl;
		if (fd > -1)
			close(fd);
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
