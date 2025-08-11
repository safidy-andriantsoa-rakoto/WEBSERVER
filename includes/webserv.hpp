/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:12 by barandri          #+#    #+#             */
/*   Updated: 2025/08/11 20:05:17 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include "general.hpp"

#include "../srcs/class/Server.hpp"
#include "../srcs/class/Request.hpp"
#include "../srcs/class/Response.hpp"
#include "../srcs/class/EpollCreation.hpp"
#include "../srcs/class/Client.hpp"

#include "../srcs/class/tempClass/AConfig.hpp"
#include "../srcs/class/tempClass/ALocation.hpp"
#include "../srcs/class/tempClass/MyConf.hpp"
#include "../srcs/class/tempClass/MyLoc.hpp"

typedef std::vector<Server>		vectServer;
typedef vectServer::iterator	vectServerIt;

//Server handler
void			server_create(Server & server);
void			initAllServer(vectServer & allServer);
void            launchEpoll(vectServer & allServer, EpollCreation &epoll);
vectServerIt	findFd(vectServer & allServer, int const & fd);

// Server main
void    		launchServer(vectServer & allServer);

//Waiting for real parser
vectServer 		parseConfigFile();

#endif