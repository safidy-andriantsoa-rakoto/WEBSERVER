/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/11/18 13:08:15 by rsafidy-         ###   ########.fr       */
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
		{
			if (checkFileExtension(argv[1], ".conf") == false)
				throw std::runtime_error("The configuration file extension must be .conf");
			if (checkIfFileAvailabe(argv[1], O_RDONLY) == false)
				throw std::runtime_error("The configuration file is not reachable or is not a file");
			config = getContentFile(argv[1]);
		}
		getAllServerFromConfigFile(allServer, config);
		Mime::initMapMime(MIME_PATH);
		handler.setVectServer(allServer);
		launchServer(handler);
		handler.cleanFdAndAlloc();
	}
	catch(const std::exception& e)
	{
		std::string error = e.what();
		std::cerr << "\033[41mError : " << e.what() << "\033[0m" << std::endl;
		handler.cleanFdAndAlloc();
		if (error == "Execve error")
			return 42;
		return 1;
	}
	return 0;
}

