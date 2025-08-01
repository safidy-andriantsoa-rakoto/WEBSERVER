/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/01 09:35:28 by barandri         ###   ########.fr       */
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
		request.parseRequest(clientRequest, server.getLocation(), server.getGeneralRoot());
		fd = server.get_fdClient();
		if (request.getMethod() == "GET")
		{
			server.get_request(request, response);
			responseString = response.responseGenerator(request.getRequestTarget());
			send_value(fd, responseString);
		}
		else if (request.getMethod() == "POST")
			std::cout << request.getMethod() << std::endl; // Calling post_request
		else if (request.getMethod() == "DELETE")
			std::cout << request.getMethod() << std::endl; // Calling delete_request
		else
			generateErrorResponse(fd, 501);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Process Request error \n" << e.what() << std::endl;
		generateErrorResponse(fd, 404); //Class exception?? //Try catch if needed
		return ;
	}
}

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
			processRequest(clientRequest, server);
			close(server.get_fdClient());
		}
	}
}
