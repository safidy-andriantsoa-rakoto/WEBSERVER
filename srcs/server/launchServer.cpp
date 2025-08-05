/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/05 09:28:32 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void    processRequest(std::string const & clientRequest, Server & server, int& fd)
{
	// int         fd;
	Request		request;
	Response	response;
    std::string responseString;

	if (!clientRequest.length())
		return ;
	std::cout << clientRequest << std::endl;
	try
	{
		request.parseRequest(clientRequest, server.getLocation(), server.getGeneralRoot());
		// fd = server.get_fdClient();
		if (request.getMethod() == "GET")
		{
			server.get_request(request, response);
			responseString = response.responseGenerator(request.getRequestTarget());
			std::cout << "Dest->fd=" << fd << std::endl;
			send_value(fd, responseString);
		}
		else if (request.getMethod() == "POST")
			std::cout << request.getMethod() << std::endl; // Calling post_request
		else if (request.getMethod() == "DELETE")
			std::cout << request.getMethod() << std::endl; // Calling delete_request
		else
			generateErrorResponse(fd, 501);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Process Request error \n" << e.what() << std::endl;
		generateErrorResponse(fd, 404); //Class exception?? //Try catch if needed
		return ;
	}
}


static int server_create(Server &server, int port)
{
	if (!server.create_socket())
		return (0);
	server.configure_sockaddrin(port);
	if (!server.link_socket_adress())
		return (0);
	if (!server.wait_connexion_from_client())
		return (0);
	return (1);
}

std::string get_client_request(int fdCli)
{
    char response[_BUFFER];
    int _respStatus = recv(fdCli, response, sizeof(response), 0);
    if (_respStatus == -1)
    {
        std::cout << "Error\nreceive a message from a socket" << std::endl;
        return ("");
    }
	std::cout << "Ecoute sur fd=" << fdCli << std::endl;
    return (response);
}

void    launchServer()
{
	Server server;

	server.simpleInit(); //For test only !!!!!!!!

	struct epoll_event max_event[10];

	int stat1 = server_create(server, 2003);
	if (!stat1)
	{
		std::cout << "Error\nFailed to load server1" << std::endl;
		return ;
	}
	int efds = epoll_create1(0);
	if (efds < 0)
	{
		perror("epoll_create: ");
		return ;
	}
	int flags = fcntl(server.get_fdSocket(), O_NONBLOCK, 0);
	fcntl(server.get_fdSocket(), F_SETFL, flags | O_NONBLOCK);// socket non bloquant
	struct epoll_event event;
	event.events = EPOLLIN | EPOLLOUT;
	event.data.fd = server.get_fdSocket();
	if(epoll_ctl(efds, EPOLL_CTL_ADD, server.get_fdSocket(), &event) < 0)
	{
		perror("EPOLL_CTRL:");
		return ;
	}
	while (1)
	{
		int ewait = epoll_wait(efds, max_event, 10, -1);
		for (int i = 0; i < ewait; i++)
		{
			std::cout << "	LE FD:" << max_event[i].data.fd << std::endl;
			if (max_event[i].data.fd == server.get_fdSocket())
			{
				std::cout << "ACCEPTER" << std::endl;
				struct sockaddr_in cli;
				cli.sin_family = AF_INET;
				cli.sin_port = htons(2003);
				cli.sin_addr.s_addr = INADDR_ANY;
				size_t size = sizeof(cli);
				int fd_cli = accept(max_event[i].data.fd, (sockaddr *)&(cli), (socklen_t *)&(size));
				if (fd_cli < 0)
				{
					std::cout << "accept error" << std::endl;
					continue;
				}
				struct epoll_event client;
				client.events = EPOLLIN | EPOLLOUT;
				client.data.fd = fd_cli;
				epoll_ctl(efds, EPOLL_CTL_ADD, fd_cli, &client);
			}
			else
			{
				std::cout << "ATY ve?" << std::endl;
				int fd =  max_event[i].data.fd;
				processRequest(get_client_request(max_event[i].data.fd), server, fd);
				epoll_ctl(efds, EPOLL_CTL_DEL, max_event[i].data.fd, NULL);
				close(max_event[i].data.fd);
			}
		}
	}
	// if (!server.create_socket())
	// 	return ;
	// server.configure_sockaddrin(2003);
	// if (!server.link_socket_adress())
	// 	return ;
	// if (!server.wait_connexion_from_client())
	// 	return ;
	// while (1)
	// {
	// 	if (server.accept_client_connexion())
	// 	{
	// 		std::string	clientRequest;

	// 		std::cout << "Connected!" << std::endl;
	// 		clientRequest = server.get_client_request();
	// 		processRequest(clientRequest, server, server.get_fdClient());
	// 		close(server.get_fdClient());
	// 	}
	// }
}
