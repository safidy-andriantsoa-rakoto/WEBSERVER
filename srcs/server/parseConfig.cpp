/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 10:19:31 by barandri          #+#    #+#             */
/*   Updated: 2025/08/11 21:07:30 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

vectServer parseConfigFile()
{
	vectServer	result;
	Server		server1;
	Server		server2;
	Server		server3;
	Server		server4;

	server1.simpleInit(2003, "./config/dim.txt");
	server2.simpleInit(2004, "./config/agency.txt");
	server3.simpleInit(2005, "./config/clean.txt");
	server4.simpleInit(2006, "./config/sample.txt");
	result.push_back(server1);
	result.push_back(server2);
	result.push_back(server3);
	result.push_back(server4);
	return result;
}