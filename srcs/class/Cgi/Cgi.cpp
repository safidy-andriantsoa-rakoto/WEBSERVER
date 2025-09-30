/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:33:08 by barandri          #+#    #+#             */
/*   Updated: 2025/09/30 11:03:31 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

stringMap Cgi::_cgiConfig;

Cgi::Cgi(){}

Cgi::Cgi(Cgi const &other)
{
	*this = other;
}

Cgi &Cgi::operator=(Cgi const &other)
{
	if (this != &other)
	{
		_reqMethod = other._reqMethod;
		_query = other._query;
		_scriptFileName = other._scriptFileName;
		_scriptName = other._scriptName;
		_contentLength = other._contentLength;
		_envp = other._envp;
		_cgiFd = other._cgiFd;
		_clientFd = other._clientFd;
	}
	return *this;
}

Cgi::~Cgi(){}

char **Cgi::getEnvp()
{
	return _envp;
}

void Cgi::parseCgiEnv(std::string const & requestTarget, std::string const & rootDirectory)
{
	stringVect	vect;

	vect = splitString(requestTarget, "?");
	_query = vect[1];
	_scriptName = vect[0];
	_scriptFileName = rootDirectory + _scriptName;
}

void Cgi::configEnvp()
{
	stringMap	ownedConfig = Cgi::_cgiConfig;
	
	std::cout << "Config ENVP\n";
	ownedConfig["REQUEST_METHOD"] = _reqMethod;
	ownedConfig["QUERY_STRING"] = _query;
	ownedConfig["SCRIPT_FILENAME"] = _scriptFileName;
	ownedConfig["SCRIPT_NAME"] = _scriptName;
	ownedConfig["CONTENT_LENGTH"] = _contentLength;
	_envp = convertStringMapToTable(ownedConfig);
	displayCharTable(_envp, "ENVP");
	clearCharTable(_envp);
}

void Cgi::execCgi(){}

void Cgi::setReqMethod(std::string const &method)
{
	_reqMethod = method;
}

void Cgi::setContentLength(size_t const &length)
{
	_contentLength = length;
}

void Cgi::displayCgiClass() const
{
	std::cout << "\033[46m==================CGI COMPONENT==================" << std::endl;
	std::cout << "Method = ["<< _reqMethod << "]" << std::endl
	<< "Query = [" << _query << "]" << std::endl
	<< "ScriptFileName = [" << _scriptFileName << "]" << std::endl
	<< "ScriptName = [" << _scriptName << "]" << std::endl
	<< "Content Length = [" << _contentLength << "]" << std::endl;
	std::cout << "==============END OF CGI COMPONENT=============\033[0m" << std::endl;
}

void Cgi::initCgiConfig(std::string const & path)
{
	_cgiConfig = splitFileToMapString(path, "=");
}
