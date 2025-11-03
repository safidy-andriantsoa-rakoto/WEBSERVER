/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:33:08 by barandri          #+#    #+#             */
/*   Updated: 2025/11/01 20:01:43 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiParser.hpp"

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

char **CgiParser::getEnvp(){return _envp;}

void CgiParser::parseCgiRequestTarget(std::string const & requestTarget)
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
}

void CgiParser::buildEnvp()
{
	std::string	ownedConfig = CGI_ENVP_CONF;
	stringVect	vect;

	replaceString(ownedConfig, _reqMethod, "$1");
	replaceString(ownedConfig, _scriptFileName, "$2");
	replaceString(ownedConfig, _scriptName, "$3");
	replaceString(ownedConfig, "http:localhost", "$4"); // REVIEW REQUEST_URI IF NECESSARY
	replaceString(ownedConfig, _query, "$5");
	replaceString(ownedConfig, convertIntToString(_contentLength), "$6");
	replaceString(ownedConfig, _contentType, "$7");
	vect = splitString(ownedConfig, "\n");
	clearCharTable(_envp);
	_envp = convertStringVectToTable(vect);
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

void CgiParser::setCgiExecPath(std::string const &path){_execPath = path;}

void CgiParser::setReqMethod(std::string const &method){_reqMethod = method;}

void CgiParser::setContentLength(size_t const &length){_contentLength = length;}

void CgiParser::setContentType(std::string const &type){_contentType = type;}

std::string CgiParser::getScriptFileName() const{return _scriptFileName;}

void CgiParser::clearCgiClass()
{
	_reqMethod.clear();
	_query.clear();
	_scriptFileName.clear();
	_scriptName.clear();
	_contentType.clear();
	_execPath.clear();
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

bool CgiParser::checkIfFileCgi(std::string const &path)
{
	size_t		pos;
	size_t		lastSlashPos;
	std::string	ext = ".php";
	stringVect	sub;

	if (checkFileExtension(path, ext) == true)
		return true;
	pos = path.find(ext);
	if (pos == std::string::npos)
		return false;
	lastSlashPos = path.find_last_of("/");
	if (pos < lastSlashPos)
		return false;
	sub = splitString(path, "?");
	if ((sub.size() == 1 && path[path.size() - 1] == '?') || sub.size() == 2)
		return true;
    return false;
}
