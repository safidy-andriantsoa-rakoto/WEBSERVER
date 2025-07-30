/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/07/30 21:11:34 by barandri         ###   ########.fr       */
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
		// server.simpleInit();
		// stringMap	map = server.getLocation();
		// stringMapIt it = findLongestMatch("/", map);
		// std::cout << "LOCATION SELECTED = " << (*it).first << " => " << (*it).second << std::endl;
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}