/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:12 by barandri          #+#    #+#             */
/*   Updated: 2025/09/16 21:47:37 by trahanta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "general.hpp"
#include "class.hpp"

// Server handler
void server_create(Server &server);
void initServer(vectServer &allServer);
void launchEpoll(vectServer &allServer, EpollCreation &epoll);

void handleClient(Handler &handler, int const &index, EpollCreation &epoll);
void handleRequest(Handler &handler, int const &index, EpollCreation &epoll, int const &indexEvent);
void handleFileIO(Handler &handler, int const &index, EpollCreation &epoll, int const &evIndex);

vectServerIt findFdInServer(vectServer &allServer, int const &fd);
VClientIt findFdInClient(VClient &allClient, int const &fd);

// Server main
void launchServer(Handler &handler);

// Waiting for real parser
vectServer parseConfigFile();

vectServer getServer(stringVect file);
vectLocation getLocation(size_t countLocation);

#endif