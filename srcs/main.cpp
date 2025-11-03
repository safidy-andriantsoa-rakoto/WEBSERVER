/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/11/03 13:03:51 by trahanta         ###   ########.fr       */
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
	vectServer	childServer;

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
		Server::getParsAllServer(allServer, config);//vita ny parse
		if(!Server::checkDuplicateValue(allServer))
			return (1);
		//vita !erreur ny parse
		for (size_t i = 0; i < allServer.size(); i++)
		{
			intVect port_tmp = allServer[i].getConfig().getServPort();
			for (size_t j = 1; j < port_tmp.size(); j++)
			{
				Server child(allServer[i]);
				child.config_ref().init_port_child(port_tmp[j]);
				childServer.push_back(child);
			}
			allServer[i].config_ref().delete_port_child_in_parent();
		}
		if (childServer.size())
			std::copy(childServer.begin(), childServer.end(), std::back_inserter(allServer));
		for (size_t i = 0; i < allServer.size(); i++)
			allServer[i].getConfig().displayConfig();
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


