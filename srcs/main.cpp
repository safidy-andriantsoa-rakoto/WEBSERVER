/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/11/03 19:54:59 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void	initProgram()
{
	Mime::initMapMime(MIME_PATH);
	DIR *dir = openDir(UPLOAD_PATH);
	closedir(dir);
}

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
		handler.cleanFdAndAlloc();
	}
	catch(const std::exception & e)
	{
		std::cerr << "\033[41mMain Function : " << e.what() << "\033[0m" << std::endl;
		std::string error = e.what();
		if (error == "Execve error")
			return 42;
		handler.cleanFdAndAlloc();
		return 1;
	}
	return 0;
}

