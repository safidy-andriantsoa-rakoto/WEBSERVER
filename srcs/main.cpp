/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/11/03 15:21:50 by rsafidy-         ###   ########.fr       */
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
	vectServer	childServer;

	try
	{		
		if (argc == 1)
		{
			std::cout << DEFAULT_CONFIG << std::endl;	
			config = splitString(DEFAULT_CONFIG, "\n");
		}
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
		std::cout << "============================================================================-----\n";
		for (size_t i = 0; i < allServer.size(); i++)
			allServer[i].getConfig().displayConfig();
		std::cout << "============================================================================-----\n";
		initProgram();
		handler.setVectServer(allServer);
		launchServer(handler);
		std::cout << "\n\n\n\nWebserv shutting down..." << std::endl;
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


