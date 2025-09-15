/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/09/15 08:08:22 by barandri         ###   ########.fr       */
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
		handler.closeAllFd();
	}
	return 0;
}
