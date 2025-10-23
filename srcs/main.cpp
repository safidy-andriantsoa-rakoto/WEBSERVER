/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/10/22 22:48:32 by trahanta         ###   ########.fr       */
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
	FileFd::initSockFd();
	LDirExcept::initPart(LDIR_PATH);
	CgiParser::initCgiConfig(CGI_PATH, CGI_ERR_PATH);
	int fd = openFile(ERR_PATH);
	close(fd);
	DIR *dir = openDir(UPLOAD_PATH);
	closedir(dir);
}

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
		
		file = getContentFile("config/serverConf/test.conf");
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
		// handler.setVectServer(allServer);
		// launchServer(handler);
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		handler.cleanFdAndAlloc();
		return -1;
	}
	return 0;
}
