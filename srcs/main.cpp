/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/07/31 15:07:13 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main()
{
	try
	{
		Response::initAllStatusCode("response/statusCode.txt");
		launchServer();

		// Server server;
		// Request request;

		// server.simpleInit();
		// request.setRequestTarget("/hello.txt", server.getLocation(), server.getGeneralRoot());
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}