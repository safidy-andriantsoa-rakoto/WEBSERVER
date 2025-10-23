/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:33:08 by barandri          #+#    #+#             */
/*   Updated: 2025/10/23 10:42:04 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiParser.hpp"

std::string CgiParser::_cgiGetConfig;
std::string CgiParser::_cgiPostConfig;

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
		_contentType = other._contentType;
		_envp = other._envp;
	}
	return *this;
}

CgiParser::~CgiParser(){}

char **CgiParser::getEnvp()
{
	return _envp;
}

void CgiParser::parseCgiEnv(std::string const & requestTarget)
{
	stringVect	vect;

	vect = splitString(requestTarget, "?");
	if (vect.size() == 2)
	{
		_query = vect[1];
	}
	size_t pos = vect[0].find_last_of("/");
	_scriptName = vect[0].substr(pos, vect[0].size() - pos);
	_scriptFileName = vect[0];
	std::cout << "CGI SCRIPT = " << _scriptFileName << " ; " << _scriptName << std::endl;
}

void CgiParser::configGetEnvp()
{
	std::string	ownedConfig = CgiParser::_cgiGetConfig;
	stringVect	vect;

	replaceString(ownedConfig, _reqMethod, "$1");
	replaceString(ownedConfig, _query, "$2");
	replaceString(ownedConfig, _scriptFileName, "$3");
	replaceString(ownedConfig, _scriptName, "$4");
	replaceString(ownedConfig, "http:localhost", "$5");
	vect = splitString(ownedConfig, "\n");
	clearCharTable(_envp);
	_envp = convertStringVectToTable(vect);
	displayCharTable(_envp, "ENVP GET TABLE");
	vect.clear();
	ownedConfig.clear();
}

void CgiParser::configPostEnvp()
{
	std::string	ownedConfig = CgiParser::_cgiPostConfig;
	stringVect	vect;

	replaceString(ownedConfig, _reqMethod, "$1");
	replaceString(ownedConfig, _scriptFileName, "$2");
	replaceString(ownedConfig, _scriptName, "$3");
	replaceString(ownedConfig, convertIntToString(_contentLength), "$4");
	if (_contentType.size())
		replaceString(ownedConfig, _contentType, "$5");
	else
	{
		std::string cType = "CONTENT-TYPE=$5";
		ownedConfig.erase(ownedConfig.find(cType.c_str()), cType.size());
	}
	replaceString(ownedConfig, "http:localhost", "$6");
	vect = splitString(ownedConfig, "\n");
	clearCharTable(_envp);
	_envp = convertStringVectToTable(vect);
	displayCharTable(_envp, "ENVP POST TABLE");
	vect.clear();
	ownedConfig.clear();
}

void CgiParser::execCgi()
{
	char	*argv[3];

	argv[0] = const_cast<char * >(_execPath.c_str());
	argv[1] = const_cast<char * >(_scriptFileName.c_str());
	argv[2] = NULL;
	if (execve(_execPath.c_str(), argv, _envp) == -1)
	{
		perror("Execve");
		clearCgiClass();
		throw std::runtime_error("Execve error");
	}
}

void CgiParser::setCgiExecPath(std::string const &path)
{
	_execPath = path;
}

void CgiParser::setReqMethod(std::string const &method)
{
	_reqMethod = method;
}

void CgiParser::setContentLength(size_t const &length)
{
	_contentLength = length;
}

void CgiParser::setContentType(std::string const &type)
{
	_contentType = type;
}

std::string CgiParser::getScriptFileName() const
{
    return _scriptFileName;
}

void CgiParser::clearCgiClass()
{
	_reqMethod.clear();
	_query.clear();
	_scriptFileName.clear();
	_scriptName.clear();
	_contentLength = 0;
	clearCharTable(_envp);
	_envp = NULL;
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

void CgiParser::initCgiConfig(std::string const & getCgiPath, std::string const & postCgiPath)
{
	_cgiGetConfig = readFileToString(getCgiPath);
	_cgiPostConfig = readFileToString(postCgiPath);
}
