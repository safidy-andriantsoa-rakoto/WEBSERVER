/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:12 by barandri          #+#    #+#             */
/*   Updated: 2025/09/09 09:18:42 by rsafidy-         ###   ########.fr       */
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
vectServer  getServer(stringVect file);

#endif