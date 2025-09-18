/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:12 by barandri          #+#    #+#             */
/*   Updated: 2025/09/18 11:20:44 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include "general.hpp"
#include "class.hpp"

//Server handler
void	server_create(Server & server);
void	initServer(vectServer & allServer);
void	launchEpoll(vectServer & allServer, EpollCreation &epoll);

void	handleServerEvent(Handler & handler, int const & i, EpollCreation & epoll);
void	handleClientEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx);
void	handleFileEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx);

// void        send_value(int const & _acceptStatus, std::string & _send_content, VClientIt itCli);

vectServerIt	findFdInServer(vectServer & allServer, int const & fd);
VClientIt		findFdInClient(VClient & allClient, int const & fd);

// Server main
void    launchServer(Handler & handler);

//Waiting for real parser
vectServer	parseConfigFile();

#endif