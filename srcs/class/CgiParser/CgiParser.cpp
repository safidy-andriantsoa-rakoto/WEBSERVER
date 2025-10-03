/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:33:08 by barandri          #+#    #+#             */
/*   Updated: 2025/10/03 08:17:10 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiParser.hpp"

std::string CgiParser::_cgiConfig;

CgiParser::CgiParser() : _envp(NULL){}

CgiParser::CgiParser(CgiParser const &other)
{
	*this = other;
}

CgiParser &CgiParser::operator=(CgiParser const &other)
{
	if (this != &other)
	{
		_reqMethod = other._reqMethod;
		_query = other._query;
		_scriptFileName = other._scriptFileName;
		_scriptName = other._scriptName;
		_contentLength = other._contentLength;
		_envp = other._envp; //Deep copy?
	}
	return *this;
}

CgiParser::~CgiParser(){}

char **CgiParser::getEnvp()
{
	return _envp;
}

void CgiParser::parseCgiEnv(std::string const & requestTarget, std::string const & rootDirectory)
{
	stringVect	vect;

	vect = splitString(requestTarget, "?");
	_query = vect[1];
	_scriptName = vect[0];
	_scriptFileName = rootDirectory + _scriptName;
}

void CgiParser::configEnvp()
{
	std::string	ownedConfig = CgiParser::_cgiConfig;
	stringVect	vect;
	
	replaceString(ownedConfig, _reqMethod, "$1");
	replaceString(ownedConfig, _query, "$2");
	replaceString(ownedConfig, _scriptFileName, "$3");
	replaceString(ownedConfig, _scriptName, "$4");
	replaceString(ownedConfig, convertIntToString(_contentLength), "$5");
	vect = splitString(ownedConfig, "\n");
	_envp = convertStringVectToTable(vect);
	displayCharTable(_envp, "ENVP");
	// clearCharTable(_envp);
	vect.clear();
	ownedConfig.clear();
}

void CgiParser::execCgi()
{
	char		*argv[3];
	std::string	php;

	php = "/usr/bin/php-cgi";
	argv[0] = const_cast<char * >(php.c_str());
	argv[1] = const_cast<char * >(_scriptFileName.c_str());
	argv[2] = NULL;
	if (execve(php.c_str(), argv, _envp) == -1)
	{
		perror("Execve");
		clearCgiClass();
		throw std::runtime_error("Execve error");
	}
}

void CgiParser::setReqMethod(std::string const &method)
{
	_reqMethod = method;
}

void CgiParser::setContentLength(size_t const &length)
{
	_contentLength = length;
}

void CgiParser::clearCgiClass()
{
	_reqMethod.clear();
	_query.clear();
	_scriptFileName.clear();
	_scriptName.clear();
	_contentLength = 0;
	clearCharTable(_envp);
}

void CgiParser::displayCgiClass() const
{
	std::cout << "\033[46m==================CGI COMPONENT==================" << std::endl;
	std::cout << "Method = ["<< _reqMethod << "]" << std::endl
	<< "Query = [" << _query << "]" << std::endl
	<< "ScriptFileName = [" << _scriptFileName << "]" << std::endl
	<< "ScriptName = [" << _scriptName << "]" << std::endl
	<< "Content Length = [" << _contentLength << "]" << std::endl;
	std::cout << "==============END OF CGI COMPONENT=============\033[0m" << std::endl;
}

void CgiParser::initCgiConfig(std::string const & path)
{
	_cgiConfig = readFileToString(path);
}
