/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/10/28 09:47:50 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/*
	Check every configFile needed by the program !!!
*/
static void	initProgram()
{
	Mime::initMapMime(MIME_PATH);
	DIR *dir = openDir(UPLOAD_PATH);
	closedir(dir);
}

// int main()
// {
// 	Handler handler;

// 	try
// 	{
// 		initProgram();
// 		vectServer	allServer = myParseConfigFile();
// 		handler.setVectServer(allServer);
// 		launchServer(handler);
// 		handler.cleanFdAndAlloc();
// 	}
// 	catch(const std::exception & e)
// 	{
// 		std::cerr << "Main exception :" << e.what() << std::endl;
// 		handler.cleanFdAndAlloc();
// 		return 1;
// 	}
// 	return 0;
// }

int main(int argc, char **argv)
{
	Handler 	handler;
	stringVect	config;
	vectServer	allServer;

	try
	{		
		if (argc == 1)
			config = splitString(DEFAULT_CONFIG, "\n");
		else
			config = getContentFile(argv[1]);
		if (!checkBraceClose(config)
				|| !checkBracesAfterNewline(config)
				|| !checkWriteConfig(config, "server", "server {")
				|| !checkNumberAllContent(config))
			return (1);
		allServer = getServer(config);
		Server::getParsAllServer(allServer, config);
		if(!Server::checkDuplicateValue(allServer))
			return (1);
		initProgram();
		handler.setVectServer(allServer);
		launchServer(handler);
		std::cout << "\n\n\n\nWebserv shutting down..." << std::endl;
		handler.cleanFdAndAlloc();
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		handler.cleanFdAndAlloc();
		return 1;
	}
	return 0;
}


