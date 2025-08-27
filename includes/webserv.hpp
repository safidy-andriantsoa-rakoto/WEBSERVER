/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:12 by barandri          #+#    #+#             */
/*   Updated: 2025/08/27 14:16:39 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include "general.hpp"
#include "class.hpp"

//Server handler
void			server_create(Server & server);
void			initAllServer(vectServer & allServer);
void            launchEpoll(vectServer & allServer, EpollCreation &epoll);
vectServerIt	findFdInServer(vectServer & allServer, int const & fd);
VClientIt	    findFdInClient(VClient & allClient, int const & fd);

// Server main
void    		launchServer(vectServer & allServer);

//Waiting for real parser
vectServer 		parseConfigFile();

#endif