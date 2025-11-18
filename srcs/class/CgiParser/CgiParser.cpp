/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:33:08 by barandri          #+#    #+#             */
/*   Updated: 2025/11/18 09:27:09 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiParser.hpp"

void CgiParser::addTransformHeader(stringMap const & header)
{
	std::string	key;

	for(stringMapIt it = header.begin(); it != header.end(); it++)
	{
		key = (*it).first;
		for (size_t i = 0; i < key.size(); i++)
		{
			if (key[i] == ' ' || key[i] == '-')
				key[i] = '_';
			else
				key[i] = std::toupper(key[i]);
		}
		_envpVect.push_back("HTTP_" + key + "=" + (*it).second);
	}
}

CgiParser::CgiParser() : _envp(NULL) {}

CgiParser::CgiParser(CgiParser const &other){*this = other;}

CgiParser &CgiParser::operator=(CgiParser const &other)
{
	if (this != &other)
	{
		_reqMethod = other._reqMethod;
		_scriptFileName = other._scriptFileName;
		_contentType = other._contentType;
		_contentLength = other._contentLength;
		_port = other._port;
		_execPath = other._execPath;
		_envpVect = other._envpVect;
		_envp = other._envp;
	}
	return *this;
}

CgiParser::~CgiParser(){}

char **CgiParser::getEnvp(){return _envp;}

void CgiParser::parseCgiURI(std::string const & host, std::string const & originReqTarget, std::string const & root)
{
	stringVect	vect;
	std::string	scriptName;
	std::string	pathInfo;
	std::string	ext = ".php";
	size_t		posQuery;

	_envpVect.push_back("REQUEST_URI=" + originReqTarget);
	size_t pos = originReqTarget.find(ext);
	pos += ext.size();
	scriptName = originReqTarget.substr(0, pos);
	_envpVect.push_back("SCRIPT_NAME=" + scriptName);
	_scriptFileName = root + scriptName;
	_envpVect.push_back("SCRIPT_FILENAME=" + _scriptFileName);
	posQuery = originReqTarget.find("?");
	if (posQuery != std::string::npos)
	{
		vect = splitString(originReqTarget, "?");
		if (vect.size() == 2)
			_envpVect.push_back("QUERY_STRING=" + vect[1]);
		vect.clear();
	}
	if (pos != std::string::npos && (pos < originReqTarget.size()))
	{
		if (posQuery != std::string::npos)
			pathInfo = originReqTarget.substr(pos, posQuery - pos);
		else
			pathInfo = originReqTarget.substr(pos, originReqTarget.size() - pos);
		if (pathInfo.size())
		{
			_envpVect.push_back("PATH_INFO=" + pathInfo);
			_envpVect.push_back("PATH_TRANSLATED=" + root + pathInfo);
		}
	}
	vect = splitString(host, ":");
	_envpVect.push_back("SERVER_NAME=" + vect[0]);
}

void CgiParser::addMethodAndPortEnvp(std::string const &method, int const &port)
{
	_envpVect.push_back("REQUEST_METHOD=" + method);
	_envpVect.push_back("SERVER_PORT=" + convertIntToString(port));
}

void CgiParser::addContentRelatedEnvp(std::string const &type, size_t const &size)
{
	_envpVect.push_back("CONTENT_TYPE=" + type);
	_envpVect.push_back("CONTENT_LENGTH=" + convertIntToString(size));
}

void CgiParser::buildEnvp()
{
	_envpVect.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_envpVect.push_back("REDIRECT_STATUS=200");
	_envpVect.push_back("SERVER_PROTOCOL=HTTP/1.1");
	std::sort(_envpVect.begin(), _envpVect.end());
	_envp = convertStringVectToTable(_envpVect);
	_envpVect.clear();
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

void CgiParser::setPort(int const &port){_port = convertIntToString(port);}

std::string CgiParser::getScriptFileName() const{return _scriptFileName;}

void CgiParser::clearCgiClass()
{
	_reqMethod.clear();
	_scriptFileName.clear();
	_contentType.clear();
	_execPath.clear();
	_contentLength = 0;
	clearCharTable(&_envp);
	_envpVect.clear();
}

void CgiParser::displayCgiClass() const
{
	std::cout << "\033[46m==================CGI COMPONENT==================" << std::endl;
	std::cout << "Method = ["<< _reqMethod << "]" << std::endl
	<< "ScriptFileName = [" << _scriptFileName << "]" << std::endl
	<< "Content Length = [" << _contentLength << "]" << std::endl
	<< "Content Length = [" << _port << "]" << std::endl;
	std::cout << "==============END OF CGI COMPONENT=============\033[0m" << std::endl;
}

bool CgiParser::checkIfFileCgi(std::string const &path)
{
	size_t		pos;
	std::string	ext = ".php";
	stringVect	sub;

	if (checkFileExtension(path, ext) == true)
		return true;
	pos = path.find(ext);
	if (pos == std::string::npos)
		return false;
    return true;
}
