/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/10/22 10:48:28 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/*
	Check every configFile needed by the program !!!
*/
static void	initProgram()
{
	Response::initAllStatusCode(STAT_PATH);
	Mime::initMapMime(MIME_PATH);
	LDirExcept::initPart(LDIR_PATH);
	CgiParser::initCgiConfig(CGI_GET_PATH, CGI_POST_PATH);
	int fd = openFile(ERR_PATH);
	close(fd);
	DIR *dir = openDir(UPLOAD_PATH);
	closedir(dir);
}

int main()
{
	Handler handler;

	try
	{
		initProgram();
		vectServer	allServer = myParseConfigFile();
		handler.setVectServer(allServer);
		launchServer(handler);
		handler.cleanFdAndAlloc();
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		handler.cleanFdAndAlloc();
		return -1;
	}
	return 0;
}

// int main(int argc, char **argv)
// {
// 	Handler handler;

// 	(void ) argc;
// 	(void) argv;
// 	try
// 	{
// 		initProgram();
// 		stringVect		file;
// 		vectLocation	Location;
// 		vectServer 		server;
		
// 		file = getContentFile("config/serverConf/test.conf");
// 		if (!checkBraceClose(file))
// 			return (1);
// 		if(!checkBracesAfterNewline(file))
// 			return (1);
// 		if (!checkWriteConfig(file, "server", "server {"))
// 			return (1);
// 		if(!checkNumberAllContent(file))
// 			return (1);
// 		vectServer	allServer = getServer(file);
// 		Server::getParsAllServer(allServer, file);
// 		// handler.setVectServer(allServer);
// 		// launchServer(handler);
// 	}
// 	catch(const std::exception & e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 		handler.cleanFdAndAlloc();
// 		return -1;
// 	}
// 	return 0;
// }
