/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/07 14:56:27 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static std::string get_client_request(int fdCli)
{
    char response[_BUFFER];
    int _respStatus = recv(fdCli, response, sizeof(response), 0);
    if (_respStatus == -1)
    {
        std::cout << "Error\nreceive a message from a socket" << std::endl;
        return ("");
    }
    return (response);
}

static std::string	establishConnectionToClient(Server & server, int & fd_cli)
{
	size_t				size;
	std::string			request;
	struct sockaddr_in	cli;

	cli.sin_family = AF_INET;
	cli.sin_port = htons(server.getPort());
	cli.sin_addr.s_addr = INADDR_ANY;
	size = sizeof(cli);
	fd_cli = accept(server.get_fdSocket(), (sockaddr *)&(cli), (socklen_t *)&(size));
	if (fd_cli < 0)
		throw std::runtime_error("Accept error");
	request = get_client_request(fd_cli);
	return request;
}

void    launchServer(vectServer & allServer)
{
	itVectServer		it;
	std::string			request;
	int					fd_cli;
	int					efds;
	int					ewait;
	struct epoll_event	max_event[MAX_EVENT];

	initAllServer(allServer);
	efds = launchEpoll(allServer);
	while (1)
	{
		ewait = epoll_wait(efds, max_event, MAX_EVENT, -1);
		for (int i = 0; i < ewait; i++)
		{
			int fd =  max_event[i].data.fd;
			std::cout << "	Server fd  = " << fd << std::endl;
			it = findFd(allServer, fd);
			try
			{
				request = establishConnectionToClient((*it), fd_cli);
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
				continue;
			}
			std::cout << "Client fd = " << fd_cli << " accepted" << std::endl;
			// struct epoll_event client;
			// client.events = EPOLLIN | EPOLLOUT;
			// client.data.fd = fd_cli;
			// epoll_ctl(efds, EPOLL_CTL_ADD, fd_cli, &client);
			(*it).processRequest(request, fd_cli);
			// epoll_ctl(efds, EPOLL_CTL_DEL, fd_cli, NULL);
			close(fd_cli);
		}
	}
}

