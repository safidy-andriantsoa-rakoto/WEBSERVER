/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:12 by barandri          #+#    #+#             */
/*   Updated: 2025/08/12 10:06:31 by rsafidy-         ###   ########.fr       */
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
#include "../srcs/class/Mime.hpp"

typedef std::vector<Client>     VClient;
typedef std::vector<Server>		vectServer;
typedef vectServer::iterator	itVectServer;

//Server handler
void			server_create(Server & server);
void			initAllServer(vectServer & allServer);
void            launchEpoll(vectServer & allServer, EpollCreation &epoll);
itVectServer	findFd(vectServer & allServer, int const & fd);

// Server main
void    		launchServer(vectServer & allServer);

//Waiting for real parser
vectServer 		parseConfigFile();

#endif