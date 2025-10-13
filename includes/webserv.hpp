/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:12 by barandri          #+#    #+#             */
/*   Updated: 2025/10/13 13:42:13 by barandri         ###   ########.fr       */
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
void	handleCgiEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx);

std::string	currentDateTimeString();

vectServerIt	findFdInServer(vectServer & allServer, int const & fd);
VClientIt		findFdInClient(VClient & allClient, int const & fd);

// Server main
void    launchServer(Handler & handler);

//Waiting for real parser
vectServer	parseConfigFile();

//HandleUtils
void	closeFileFdEvent(Handler & handler, int const & i, EpollCreation & epoll);
void	closeCgiFdEvent(Handler & handler, int const & i, EpollCreation & epoll);

//MessageUtils
void	sendLdirResponse(VClientIt & it, int & fdCli, EpollCreation & epoll);
void	sendSingleResponse(VClientIt & it, int & fdCli, EpollCreation & epoll);
void	sendHtmlFileResponse(VClientIt & it, int & fdCli, EpollCreation & epoll);
void	sendCgiResponse(VClientIt & it, int & fdCli, EpollCreation & epoll);
void    send_value(int const & _acceptStatus, std::string & _send_content);

#endif