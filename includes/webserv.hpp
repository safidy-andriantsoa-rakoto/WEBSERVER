/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:12 by barandri          #+#    #+#             */
/*   Updated: 2025/07/29 10:46:15 by barandri         ###   ########.fr       */
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
void	generateErrorResponse(int & fd, int const & statusCode, std::string const & reasonPhrase);

// Server main
void    processRequest(std::string const & clientRequest, Server & server);
void	launchServer();

#endif