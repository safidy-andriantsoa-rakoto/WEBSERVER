/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/11/10 19:54:21 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void manageMultiPort(vectServer & allServer)
{
	vectServer	childServer;

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
}

void	getAllServerFromConfigFile(vectServer & allServer, stringVect & config)
{
	serverVect	allServer_conf;

	parse_config_data(config, allServer_conf);
	getAllServerFromDataConf(allServer, allServer_conf);	
	manageMultiPort(allServer);
	for (size_t i = 0; i < allServer.size(); i++) /* [Test only] */
		allServer[i].getConfig().displayConfig();
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
		getAllServerFromConfigFile(allServer, config);
		Mime::initMapMime(MIME_PATH);
		handler.setVectServer(allServer);
		launchServer(handler);
		std::cout << "\n\n\n\nWebserv shutting down..." << std::endl;
		handler.cleanFdAndAlloc();
	}
	catch(const std::exception& e)
	{
		std::cerr << "\033[41mMain Function : " << e.what() << "\033[0m" << std::endl;
		std::string error = e.what();
		handler.cleanFdAndAlloc();
		if (error == "Execve error")
			return 42;
		return 1;
	}
	return 0;
}

