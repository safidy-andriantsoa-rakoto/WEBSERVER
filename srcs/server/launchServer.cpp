/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/07/28 21:02:35 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void    launchServer()
{
	Server server;

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
			Request		request;
			std::string	clientRequest;
			int			fd;

            std::cout << "Connected!" << std::endl;
            clientRequest = server.get_client_request();
            if (clientRequest.length())
            {	
                request.parseRequest(clientRequest);
                fd = server.get_fdClient();
				if (request.getMethod() == "GET")
                	server.get_request(request, fd);
                else
                    std::cout << request.getMethod() << std::endl;
            }
            close(server.get_fdClient());
        }
    }
}
