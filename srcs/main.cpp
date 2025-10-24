/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/10/24 15:43:53 by barandri         ###   ########.fr       */
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
	Handler handler;

	(void ) argc;
	(void) argv;
	try
	{
		initProgram();
		stringVect		file;
		vectLocation	Location;
		vectServer 		server;
		
		file = getContentFile("config/serverConf/default.conf");
		if (!checkBraceClose(file))
			return (1);
		if(!checkBracesAfterNewline(file))
			return (1);
		if (!checkWriteConfig(file, "server", "server {"))
			return (1);
		if(!checkNumberAllContent(file))
			return (1);
		vectServer	allServer = getServer(file);
		Server::getParsAllServer(allServer, file);
		if(!Server::checkDuplicateValue(allServer))
			return (1);
		handler.setVectServer(allServer);
		launchServer(handler);
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		handler.cleanFdAndAlloc();
		return -1;
	}
	return 0;
}
