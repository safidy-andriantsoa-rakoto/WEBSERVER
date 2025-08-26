/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   class.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 10:25:14 by barandri          #+#    #+#             */
/*   Updated: 2025/08/26 15:05:17 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLASS_HPP
# define CLASS_HPP

#include "../srcs/class/Server/Server.hpp"
#include "../srcs/class/Request/Request.hpp"
#include "../srcs/class/Response/Response.hpp"
#include "../srcs/class/Epoll/EpollCreation.hpp"
#include "../srcs/class/Client/Client.hpp"
#include "../srcs/class/Mime/Mime.hpp"

// Waiting for official class config and location
#include "../srcs/class/Config/AConfig.hpp"
#include "../srcs/class/Config/ALocation.hpp"
#include "../srcs/class/Config/MyConf.hpp"
#include "../srcs/class/Config/MyLoc.hpp"

// Put here all typedef used in all code through webserv.hpp
typedef std::vector<Client>     VClient;
typedef VClient::iterator       VClientIt;

typedef std::vector<Server>		vectServer;
typedef vectServer::iterator	vectServerIt;

#endif