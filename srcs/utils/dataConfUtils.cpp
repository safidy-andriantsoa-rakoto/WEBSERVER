/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dataConfUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 19:34:09 by barandri          #+#    #+#             */
/*   Updated: 2025/11/09 10:57:50 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	error_ligne(int const & i, std::string const & line)
{
	std::stringstream error;
	error << "\033[031;47mError: ligne[" << i + 1 << "]->" << line  << "<-\033[0m";
	throw std::runtime_error(error.str());
}

void	server_line_checker(std::istringstream & line, serverVect & allServer_conf, int const & i)
{
	std::string word;
	stringVect	line_data;
	vectData	all_data_conf;

	while (line >> word)
		line_data.push_back(word);
	if (line_data.size() != 1 || line_data[0] != "server")
		error_ligne(i, line.str());
	else
		allServer_conf.push_back(all_data_conf);
}

void	get_data_lvl(std::istringstream &line, serverVect &allServer_conf,int const & lvl, int const & i)
{
	std::string word;
	s_dataConf secLvl;

	while (line >> word)
	{
		if (!secLvl.key.size())
		{
			secLvl.key = word;
			secLvl.lvl = lvl;
		}
		else
			secLvl.value.push_back(word);
	}
	if (secLvl.key.size())
		allServer_conf[allServer_conf.size() - 1].push_back(secLvl);
	else
		error_ligne(i, line.str());
}

void	content_lvl_parser(std::string &config, int const & i, serverVect &allServer_conf, std::istringstream &line)
{
	size_t pos;
	std::string identat = "\t";
	if (config.find(identat + identat) == 0)
	{
		config.erase(0, identat.size() * 2);
		pos = config.find("\t");
		if (pos == 0)
			error_ligne(i, config);
		if (pos > 0 || pos == std::string::npos)
		{
			pos = config.find(" ");
			if (pos == 0)
				error_ligne(i, config);
			get_data_lvl(line, allServer_conf, 2, i);
		}
	}
	else if (config.find(identat) == 0)
	{
		config.erase(0, identat.size());
		pos = config.find(" ");
		if (pos == 0)
			error_ligne(i, config);
		if (pos > 0 || pos == std::string::npos)
		{
			pos = config.find(" ");
			if (pos == 0)
				error_ligne(i, config);
			get_data_lvl(line, allServer_conf, 1, i);
		}
	}
	else
		error_ligne(i, config);
}

void parse_config_data(stringVect &config, serverVect &allServer_conf)
{
	std::string identat = "\t";
	
	for (size_t i = 0; i < config.size(); i++)
	{
		size_t pos;
		std::istringstream line(config[i]);
		std::string word;

		pos = config[i].find(identat);
		if (config[i].size() == 1)
			continue;
		if (pos != 0 || pos == std::string::npos)
		{
			pos = config[i].find(" ");
			if (pos == 0)
				error_ligne(i, config[i]);
			server_line_checker(line, allServer_conf, i);
		}
		else if (pos == 0 && allServer_conf.size())
			content_lvl_parser(config[i], i, allServer_conf, line);
		else
			error_ligne(i, config[i]);
	}
	config.clear();
}

void	getAllServerFromDataConf(vectServer & allServer, serverVect & allServer_conf)
{
	intVect	allPort;
	
	for (size_t i = 0; i < allServer_conf.size(); i++)
	{
		MyConf	config;
		config.parseConfig(allServer_conf[i], allPort);
		Server server(config);
		allServer.push_back(server);
	}
	allServer_conf.clear();
}
