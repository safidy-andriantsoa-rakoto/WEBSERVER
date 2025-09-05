/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/08/29 19:28:09 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main()
{
	try
	{
		Response::initResponseUtils(STAT_PATH, ERR_PATH);
		vectServer	allServer = parseConfigFile();
		launchServer(allServer);
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
