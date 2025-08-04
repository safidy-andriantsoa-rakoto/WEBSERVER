/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/04 11:46:19 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void    launchServer()
{
	Server server;

	server.simpleInit(); //For test only !!!!!!!!

	if (!server.create_socket())
		return ;
	server.configure_sockaddrin(2003);
	if (!server.link_socket_adress())
		return ;
	if (!server.wait_connexion_from_client())
		return ;
	while (1)
	{
		if (server.accept_client_connexion())
		{
			std::string	clientRequest;

			std::cout << "Connected!" << std::endl;
			clientRequest = server.get_client_request();
			server.processRequest(clientRequest);
			close(server.get_fdClient());
		}
	}
}
