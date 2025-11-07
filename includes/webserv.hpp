/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:12 by barandri          #+#    #+#             */
/*   Updated: 2025/11/07 20:11:24 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include "general.hpp"
#include "class.hpp"

        /* PROGRAM */

//  eventHandling
void			handleServerEvent(Handler & handler, int const & i, EpollCreation & epoll);
void			handleClientEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx);
void			readCgiOutput(Handler & handler, int const & i, EpollCreation & epoll);
void			writeCgiInput(Handler & handler, int const & i, EpollCreation & epoll);

//  initServer
void			server_create(Server & server);
void			initServer(vectServer & allServer);
void			launchEpoll(vectServer & allServer, EpollCreation &epoll);
std::string		currentDateTimeString();

//  launchServer
void			manageEpollLoopError(Handler & handler, EpollCreation & epoll, int const & identifier, int const & fdEvent, int const & index, std::exception & e);
void			callEvent(Handler & handler, EpollCreation & epoll, int const & index, int const & identifier, int const & i);
void			launchServer(Handler & handler);


//  sendingResponse
int				processCgiSetup(VClientIt & it);
void			sendingResponseToClient(VClientIt & it, int & fdCli, EpollCreation & epoll);
void   			send_value(int const & _acceptStatus, std::string & _send_content);


            /*UTILS*/

// DataConfUtils
void            error_ligne(int const & i, std::string const & line);
void            server_line_checker(std::istringstream & line, serverVect & allServer_conf, int const & i);
void            get_data_lvl(std::istringstream &line, serverVect &allServer_conf,int const & lvl, int const & i);
void            content_lvl_parser(std::string &config, int const & i, serverVect &allServer_conf, std::istringstream &line);
void            parse_config_data(stringVect &config, serverVect &allServer_conf);
void	        getAllServerFromDataConf(vectServer & allServer, serverVect & allServer_conf);

//HandleUtils
void			closeCgiFdEvent(Handler & handler, int const & i, EpollCreation & epoll);
void			createCgiFd(VClientIt & it, Handler & handler, EpollCreation & epoll, int *fd, pid_t const & childPid);
void	        manageCgiAfterProcessRequest(VClientIt & it, EpollCreation & epoll, Handler & handler, int *fdCgi, pid_t childPid);
void			handleCgiError(Handler & handler, int const & index, EpollCreation & epoll, AExcept & err);
bool			hasSingleResponse(int const & respStat);
std::string		getIdentifier(int const & identifier);

//Searching Utils
vectServerIt	findFdInServer(vectServer & allServer, int const & fd);
VClientIt		findFdInClient(VClient & allClient, int const & fd);

#endif