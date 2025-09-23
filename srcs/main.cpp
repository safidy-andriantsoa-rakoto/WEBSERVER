/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/09/23 15:27:38 by barandri         ###   ########.fr       */
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
	// try
	// {
	// 	LDirExcept::initComponent();
	// 	std::string	path = "./site/agency/assets/";
	// 	DIR *dir = opendir(path.c_str());
	// 	if (!dir)
	// 	{
	// 		perror("Test");
	// 		return 0;
	// 	}
	// 	stringMap	map = listingDirectory(dir, path);
	// 	displayStringMap(map);
	// 	std::string list = convertListing(map);
	// 	std::cout << LDirExcept::_component[0] << list << LDirExcept::_component[1] << std::endl;
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
