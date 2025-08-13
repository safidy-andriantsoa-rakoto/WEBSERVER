/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   class.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 10:25:14 by barandri          #+#    #+#             */
/*   Updated: 2025/08/13 10:33:10 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLASS_HPP
# define CLASS_HPP

#include "../srcs/class/Server.hpp"
#include "../srcs/class/Request.hpp"
#include "../srcs/class/Response.hpp"
#include "../srcs/class/EpollCreation.hpp"
#include "../srcs/class/Client.hpp"
#include "../srcs/class/Mime.hpp"

// Waiting for official class config and location
#include "../srcs/class/tempClass/AConfig.hpp"
#include "../srcs/class/tempClass/ALocation.hpp"
#include "../srcs/class/tempClass/MyConf.hpp"
#include "../srcs/class/tempClass/MyLoc.hpp"

// Put here all typedef used in all code through webserv.hpp
typedef std::vector<Client>     VClient;
typedef std::vector<Server>		vectServer;
typedef vectServer::iterator	vectServerIt;

#endif