/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/09/25 08:41:29 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/*
	Check for all the default file too : 
		erropage : default/index.html
		server_conf : default_config.conf
*/
static void	initProgram()
{
	Response::initAllStatusCode(STAT_PATH);
	Mime::initMapMime(MIME_PATH);
	FileFd::initSockFd();
	LDirExcept::initPart(LDIR_PATH);
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
	}
	// try
	// {
	// 	LDirExcept::initPart();
	// 	std::string	path = "./site/agency/assets/";
	// 	std::string	relativePath = "agency/assets/";
	// 	DIR *dir = opendir(path.c_str());
	// 	if (!dir)
	// 	{
	// 		perror("Test");
	// 		return 0;
	// 	}
	// 	stringMap	map = listingDirectory(dir, relativePath);
	// 	displayStringMap(map);
	// 	std::string list = convertListing(map);
	// 	std::cout << LDirExcept::_part[0] << list << LDirExcept::_part[1] << std::endl;
	// 	list.clear();
	// 	path.clear();
	// 	map.clear();
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }
	return 0;
}
