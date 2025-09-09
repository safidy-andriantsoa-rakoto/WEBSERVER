/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/09 13:34:58 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// static void run_process(int i, EpollCreation &epoll, VClient &v_client, vectServer & allServer)
// {
// 	vectServerIt	itServ;
// 	VClientIt		itCli;
// 	int				fd;
// 	SEpollEvent		event;

// 	event = epoll.getEvent_struct(i);
// 	fd = event.data.fd;
// 	itServ = findFdInServer(allServer, fd);
// 	if (itServ != allServer.end())
// 	{
// 		int fdc;

// 		fdc = (*itServ).accept_client_connexion();
// 		while (fdc > 0)
// 		{
// 			epoll.no_blocking_fd(fdc);
// 			epoll.epoll_control(EPOLLIN, fdc);
// 			Client client(
// 				(*itServ).get_fdSocket(),
// 				fdc,
// 				(*itServ).get_confSockAddrIn(),
// 				true
// 			);
// 			v_client.push_back(client);
// 			fdc = (*itServ).accept_client_connexion();
// 		}
// 	}
// 	else
// 	{
// 		if (event.events & EPOLLIN)
// 		{
// 			itCli = findFdInClient(v_client, fd);
// 			if (itCli != v_client.end())
// 			{
// 				if ((*itCli).getConnexionStat())
// 				{
// 					try
// 					{
// 						(*itCli).ClientRequest();
// 						epoll.epoll_update_event(fd, EPOLLOUT);
// 					}
// 					catch(const std::exception& e)
// 					{
// 						epoll.epoll_clean(fd);
// 						v_client.erase(itCli);
// 						std::cerr << e.what() << '\n';
// 					}
// 				}
// 				else
// 				{
// 					epoll.epoll_clean(fd);
// 					v_client.erase(itCli);
// 				}
// 			}
// 		}
// 		else if (event.events & EPOLLOUT)
// 		{
// 			itCli = findFdInClient(v_client, fd);
// 			itServ = findFdInServer(allServer, (*itCli).get_fdparent());
// 			if (itCli != v_client.end())
// 			{
// 				if ((*itCli).getConnexionStat())
// 				{
// 					std::string client_request;
// 					try
// 					{
// 						client_request = (*itCli).getClientRequest();
// 						if (client_request.find("HTTP") != std::string::npos)
// 						{
// 							if (itServ != allServer.end())
// 								(*itServ).processRequest(client_request, fd);
// 						}
// 						client_request.clear();
// 						epoll.epoll_update_event(fd, EPOLLIN);
// 					}
// 					catch(const std::exception& e)
// 					{
// 						epoll.epoll_update_event(fd, EPOLLIN);
// 						std::cout << e.what();
// 						return ;
// 					}
// 				}
// 				else
// 				{
// 					epoll.epoll_clean(fd);
// 					v_client.erase(itCli);
// 				}
// 			}
// 		}	
// 	}
// }

int sign = -1;

void handle_c(int SIGNUM)
{
	if (SIGNUM == SIGINT)
		sign = SIGINT;
}

void    launchServer(Handler & handler)
{
	int				fdEvent;
	int				index;
	int 			nb_event;
	std::string		identifier;
	EpollCreation	epoll;

	initServer(handler.getVectServer());
	launchEpoll(handler.getVectServer(), epoll);
	signal(SIGINT, handle_c);
	while (SIGINT != sign)
	{
		nb_event = epoll.epoll_wait_event();
		index = 0;
		handler.displayHandlerStat();
		for (int i = 0; i < nb_event; i++)
		{
			fdEvent = epoll.get_fdEvent(i);
			std::cout << "FD EVENT = " << fdEvent << std::endl;
			identifier = handler.identifyFd(fdEvent, index);
			if (identifier == "SERVER")
			{
				std::cout << "SERVER le" << std::endl;
				handleClient(handler, index, epoll);
			}
			else if (identifier == "CLIENT")
			{
				std::cout << "CLIENT le" << std::endl;
				handleRequest(handler, index, epoll, i);
			}
			else if (identifier == "FILEFD")
			{
				std::cout << "FILEFD le" << std::endl;
				try
				{
					handleFileIO(handler, index, epoll, i);
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << '\n';
				}
				
			}
			else
				std::cout << "Unknown Fd !!!!" << std::endl;
		}
	}
}
