/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:12 by barandri          #+#    #+#             */
/*   Updated: 2025/08/08 10:06:21 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include "general.hpp"

#include "../srcs/class/Server.hpp"
#include "../srcs/class/Request.hpp"
#include "../srcs/class/Response.hpp"

typedef std::vector<Server>		vectServer;
typedef vectServer::iterator	itVectServer;

//Server handler
void			server_create(Server & server);
void			initAllServer(vectServer & allServer);
int	            launchEpoll(vectServer & allServer);
itVectServer	findFd(vectServer & allServer, int const & fd);

// Server main
void    		launchServer(vectServer & allServer);

//Waiting for real parser
vectServer 		parseConfigFile();

#endif