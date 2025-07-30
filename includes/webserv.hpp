/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:12 by barandri          #+#    #+#             */
/*   Updated: 2025/07/30 09:53:24 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include "general.hpp"

#include "../srcs/class/Server.hpp"
#include "../srcs/class/Request.hpp"
#include "../srcs/class/Response.hpp"

typedef std::list<Server>   ServerList;

// Error handling
void	generateErrorResponse(int & fd, int const & statusCode);

// Server main
void    processRequest(std::string const & clientRequest, Server & server);
void	launchServer();

//For test waiting for real config parsing
void    simpleInitServer(Server & server);

#endif