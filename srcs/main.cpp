/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/11/06 11:41:17 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// static void	initProgram()
// {
// 	Mime::initMapMime(MIME_PATH);
// 	DIR *dir = openDir(UPLOAD_PATH);
// 	closedir(dir);
// }

int main(int argc, char **argv)
{
	// Handler 	handler;
	stringVect	config;
	serverVect	allServer_conf;
	dataMap		_data_conf;
	// vectServer	allServer;
	// vectServer	childServer;

	(void)argc;
	(void)argv;
	
	// try
	// {		
	{
		if (argc == 1)
			config = splitString(DEFAULT_CONFIG, "\n");
		else
			config = getContentFile(argv[1]);
		for (size_t i = 0; i < config.size(); i++)
		{
			size_t pos;
			std::istringstream line(config[i]);
			parseMap _content_conf;
			std::string word;
			stringVect	line_data;
			pos = config[i].find("\t");// if faut rechercher dabort un tab pos 0
			// pos2 = config[i].find("\t");
			if (config[i].size() == 1)
			continue;
			std::cout << "=============================================================================" << std::endl;
			// while (line >> word)
			// 	line_data.push_back(word);
			if (pos == std::string::npos)
			{
				if (line_data.size() == 1) //server bloc
				{
					_data_conf.clear();
					_data_conf[line_data[0]];
					allServer_conf.push_back(_data_conf);
					std::cout << "\033[43mSize Map= "<< allServer_conf.size() << "\033[0m" << std::endl;
				}
			}
			else if (pos == 0)
			{
				config[i].erase(0, 1);// changer par rapport a la identation space or tab
				std::cout << "\033[046mLEVEL->"<< config[i] <<"<-\033[0m" << std::endl;
				pos = config[i].find("\t");
				if (pos == 0)
				{
					line_data.clear();
					std::cout << "\033[045mLEVEL 2v\033[0m" << std::endl;
					while (line >> word)
						line_data.push_back(word);
					for (size_t j = 0; j < line_data.size(); j++)
					{
						std::cout << "			WORD2->" << line_data[j] << std::endl;
					}
					std::cout << "\033[045mLEVEL 2^\033[0m" << std::endl;
				}
				else
				{
					line_data.clear();
					std::cout << "\033[044mLEVEL 1v\033[0m" << std::endl;
					while (line >> word)
						line_data.push_back(word);
					for (size_t j = 0; j < line_data.size(); j++)
					{
						std::cout << "			WORD1->" << line_data[j] << std::endl;
					}
					std::cout << "\033[044mLEVEL 1^\033[0m" << std::endl;
				}
			}

			std::cout << "Ligne["<< i <<"]->"<< config[i] << "<-size=" << config[i].size() << std::endl;
			std::cout << "=============================================================================" << std::endl;
		}
	}
	// 	if (!checkBraceClose(config)
	// 			|| !checkBracesAfterNewline(config)
	// 			|| !checkWriteConfig(config, "server", "server {")
	// 			|| !checkNumberAllContent(config))
	// 		return (1);
	// 	allServer = getServer(config);
	// 	Server::getParsAllServer(allServer, config);//vita ny parse
	// 	std::cout << "aty ee" << std::endl;
	// 	if(!Server::checkDuplicateValue(allServer))
	// 		return (1);
	// 	for (size_t i = 0; i < allServer.size(); i++)
	// 	{
	// 		intVect port_tmp = allServer[i].getConfig().getServPort();
	// 		for (size_t j = 1; j < port_tmp.size(); j++)
	// 		{
	// 			Server child(allServer[i]);
	// 			child.config_ref().init_port_child(port_tmp[j]);
	// 			childServer.push_back(child);
	// 		}
	// 		allServer[i].config_ref().delete_port_child_in_parent();
	// 	}
	// 	if (childServer.size())
	// 		std::copy(childServer.begin(), childServer.end(), std::back_inserter(allServer));
	// 	for (size_t i = 0; i < allServer.size(); i++)
	// 		allServer[i].getConfig().displayConfig();
	// 	initProgram();
	// 	handler.setVectServer(allServer);
	// 	launchServer(handler);
	// 	std::cout << "\n\n\n\nWebserv shutting down..." << std::endl;
	// 	handler.cleanFdAndAlloc();
	// }
	// catch(const std::exception & e)
	// {
	// 	std::cerr << "\033[41mMain Function : " << e.what() << "\033[0m" << std::endl;
	// 	std::string error = e.what();
	// 	if (error == "Execve error")
	// 		return 42;
	// 	handler.cleanFdAndAlloc();
	// 	return 1;
	// }
	return 0;
}

