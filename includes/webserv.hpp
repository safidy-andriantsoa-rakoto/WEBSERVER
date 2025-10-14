/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:12 by barandri          #+#    #+#             */
/*   Updated: 2025/10/14 14:53:44 by barandri         ###   ########.fr       */
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
void	readCgiOutput(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx);
void	writeCgiInput(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx);

std::string	currentDateTimeString();

vectServerIt	findFdInServer(vectServer & allServer, int const & fd);
VClientIt		findFdInClient(VClient & allClient, int const & fd);

// Server main
void	manageEpollLoopError(Handler & handler, EpollCreation & epoll, int const & identifier, int const & fdEvent, int const & index, std::exception & e);
void	callEvent(Handler & handler, EpollCreation & epoll, int const & index, int const & identifier, int const & i);
void	handleCgiError(Handler & handler, int const & index, EpollCreation & epoll, AExcept & err, int const & identifier);
int		readCgiStdError(vectCgiFdIt & itCgi, EpollCreation & epoll);
void    launchServer(Handler & handler);

//Waiting for real parser
vectServer	parseConfigFile();

//HandleUtils
void	closeFileFdEvent(Handler & handler, int const & i, EpollCreation & epoll);
void	closeCgiFdEvent(Handler & handler, int const & i, EpollCreation & epoll);
void	addFdInHandlerAndEpoll(VClientIt & it, Handler & handler, EpollCreation & epoll, int const & epollOpt, int *fdFromRequest);
int     readCgiStdError(vectCgiFdIt & itCgi, EpollCreation & epoll);
void	handleCgiError(Handler & handler, int const & index, EpollCreation & epoll, AExcept & err, int const & identifier);
bool	hasSingleResponse(int const & respStat);

//MessageUtils
void	sendSingleResponse(VClientIt & it, int & fdCli, EpollCreation & epoll);
void	sendHtmlFileResponse(VClientIt & it, int & fdCli, EpollCreation & epoll);
void	sendCgiResponse(VClientIt & it, int & fdCli, EpollCreation & epoll);
void    send_value(int const & _acceptStatus, std::string & _send_content);

#endif