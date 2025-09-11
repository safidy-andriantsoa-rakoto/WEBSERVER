/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/09/11 07:30:13 by trahanta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main()
{
		// size_t i = 0;
	std::vector<std::string> file;
	std::vector<std::vector<std::string> > test;
	vectServer server;
	Server s;
	file = getContentFile("srcs/utils/webser.conf");

		// if (!checkBraceClose(file))
	// 	return (1);
	// if(!checkBracesAfterNewline(file))
	// 	return (1);
	// if (checkWriteConfig(file, "server", "server {"))
	// 	return (1);
	// if(!checkNumberAllContent(file))
	// 	return (1);
	server = getServer(file);
	s.getParsAllServer(server, file);
	// try
	// {
	// 	Response::initResponseUtils(STAT_PATH, ERR_PATH);
	// 	Mime::initMapMime(MIME_PATH);
	// 	vectServer	allServer = parseConfigFile();
	// 	Handler		handler(allServer);
	// 	launchServer(handler);
	// }
	// catch(const std::exception & e)
	// {
	// 	std::cerr << e.what() << std::endl;
	// }
	return 0;
}

