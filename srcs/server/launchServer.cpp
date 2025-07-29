/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/07/29 10:53:27 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void    processRequest(std::string const & clientRequest, Server & server)
{
	int         fd;
	Request		request;
	Response	response;
    std::string responseString;

	if (!clientRequest.length())
		return ;
	try
	{
		request.parseRequest(clientRequest);
		fd = server.get_fdClient();
		if (request.getMethod() == "GET")
			server.get_request(request, response, fd);
		else if (request.getMethod() == "POST")
			std::cout << request.getMethod() << std::endl; // Calling post_request
		else if (request.getMethod() == "DELETE")
			std::cout << request.getMethod() << std::endl; // Calling delete_request
		else
			generateErrorResponse(fd, 501, "Not implemented");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		generateErrorResponse(fd, 404, "Not found"); //Class exception?
		return ;
	}
	//Atao eto ny fandefasana ny reponse any @ client !!!
	// responseString = response.responseGenerator();
    // send_value(fd, responseString);
}

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
			std::string	clientRequest;

			std::cout << "Connected!" << std::endl;
			clientRequest = server.get_client_request();
			processRequest(clientRequest, server);
			close(server.get_fdClient());
		}
	}
}
