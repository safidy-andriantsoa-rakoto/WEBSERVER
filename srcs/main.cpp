/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/10/14 08:45:08 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/*
	Check every configFile needed by the program !!!
	Check for upload directory 
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
}

int main()
{
	Handler handler;

	try
	{
		initProgram();
		vectServer	allServer = parseConfigFile();
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
