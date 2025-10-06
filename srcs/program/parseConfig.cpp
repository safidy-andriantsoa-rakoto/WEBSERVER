/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 10:19:31 by barandri          #+#    #+#             */
/*   Updated: 2025/09/25 10:00:54 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

vectServer parseConfigFile()
{
	vectServer	result;
	Server		server1;
	// Server		server2;
	// Server		server3;
	// Server		server4;

	server1.simpleInit("./config/serverConf/server/server2.txt", "./config/serverConf/location/sample.txt");
	// server2.simpleInit("./config/serverConf/server/server2.txt", "./config/serverConf/location/minimal.txt");
	// server3.simpleInit("./config/serverConf/server/server3.txt", "./config/serverConf/location/agency.txt");
	// server4.simpleInit("./config/serverConf/server/server1.txt", "./config/serverConf/location/sample.txt");
	result.push_back(server1);
	// result.push_back(server2);
	// result.push_back(server3);
	// result.push_back(server4);
	return result;
}