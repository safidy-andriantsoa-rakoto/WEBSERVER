/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/09/18 15:06:58 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main()
{
	Handler handler;

	try
	{
		Response::initResponseUtils(STAT_PATH, ERR_PATH);
		Mime::initMapMime(MIME_PATH);
		FileFd::initSockFd();
		vectServer	allServer = parseConfigFile();
		handler.setVectServer(allServer);
		launchServer(handler);
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		handler.cleanFdAndAlloc();
	}
	// DIR *dir = opendir("./srcs/class");
	// if (!dir)
	// {
	// 	perror("Test");
	// 	return 0;
	// }
	// stringVect	vect = listingDirectory(dir, "./srcs/class");
	// try
	// {
	// 	std::string test = readFileToString("./site/sample/favicon.ico");
	// 	// chunkingString(test);
	// 	std::cout << test << std::endl;
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }
	
	return 0;
}
