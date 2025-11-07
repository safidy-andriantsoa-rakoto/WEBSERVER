/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/11/07 11:05:18 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	initProgram()
{
	Mime::initMapMime(MIME_PATH);
	DIR *dir = openDir(UPLOAD_PATH);
	closedir(dir);
}

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

void parsingConfigFileSetup(stringVect & config, vectServer & allServer)
{
	serverVect	allServer_conf;
	vectData	all_data_conf;

	for (size_t i = 0; i < config.size(); i++)
	{
		size_t pos;
		std::istringstream line(config[i]);
		// parseMap _content_conf;
		std::string word;
		stringVect	line_data;
		pos = config[i].find("\t");// if faut rechercher dabort un tab pos 0
		if (config[i].size() == 1)
		continue;
		// std::cout << "=============================================================================" << std::endl;
		if (pos == std::string::npos)
		{
			while (line >> word)
				line_data.push_back(word);
			if (line_data.size() != 1 || line_data[0] != "server") //server bloc
			{
				std::stringstream error;
				error << "Error: ligne[" << i << "]->" << config[i];
				throw std::runtime_error(error.str());
			}
			else
			{
				all_data_conf.clear();
				allServer_conf.push_back(all_data_conf);
			}
			// std::cout << "		\033[042mSERVER NB=" << allServer_conf.size() << "\033[0m" << std::endl;
		}
		else if (pos == 0)
		{
			config[i].erase(0, 1);// changer par rapport a la identation space or tab
			// std::cout << "\033[046mLEVEL->"<< config[i] <<"<-\033[0m" << std::endl;
			pos = config[i].find("\t");
			if (pos == 0)
			{
				s_dataConf secLvl;
				// std::cout << "\033[045mLEVEL 2v\033[0m" << std::endl;
				while (line >> word)
				{
					if (!secLvl.key.size())
					{
						secLvl.key = word;
						secLvl.lvl = 2;
					}
					else
						secLvl.value.push_back(word);
				}
				allServer_conf[allServer_conf.size() - 1].push_back(secLvl);
				// std::cout << "\033[045mLEVEL 2^\033[0m" << std::endl;
			}
			else
			{
				s_dataConf firLVl;
				// std::cout << "\033[044mLEVEL 1v\033[0m" << std::endl;
				while (line >> word)
				{
					if (!firLVl.key.size())
					{
						firLVl.key = word;
						firLVl.lvl = 1;
					}
					else
						firLVl.value.push_back(word);
				}
				allServer_conf[allServer_conf.size() - 1].push_back(firLVl);
				// std::cout << "\033[044mLEVEL 1^\033[0m" << std::endl;
			}
		}
		// std::cout << "Ligne["<< i <<"]->"<< config[i] << "<-size=" << config[i].size() << std::endl;
		// std::cout << "=============================================================================" << std::endl;
	}
	// displayServerVectConfig(allServer_conf);
	intVect	allPort;

	for (size_t i = 0; i < allServer_conf.size(); i++)
	{
		MyConf	config;
		config.parseNewConfig(allServer_conf[i], allPort);
		Server server(config);
		allServer.push_back(server);
	}
}

int main(int argc, char **argv)
{
	stringVect	config;
	vectServer	allServer;
	Handler 	handler;

	try
	{
		if (argc == 1)
			config = splitString(DEFAULT_CONFIG, "\n");
		else
			config = getContentFile(argv[1]);
		parsingConfigFileSetup(config, allServer);
		config.clear();
		manageMultiPort(allServer);
		for (size_t i = 0; i < allServer.size(); i++)
			allServer[i].getConfig().displayConfig();
		// initProgram();
		// handler.setVectServer(allServer);
		// launchServer(handler);
		// std::cout << "\n\n\n\nWebserv shutting down..." << std::endl;
		// handler.cleanFdAndAlloc();
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

