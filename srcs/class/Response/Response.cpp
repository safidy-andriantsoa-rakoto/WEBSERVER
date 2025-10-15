/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:41 by barandri          #+#    #+#             */
/*   Updated: 2025/10/15 13:09:19 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

stringMap	Response::_allStatusCode;
std::string	Response::_defaultErrPage;

Response::Response() : _respCategory(STD_RSP){}

Response::Response(int const & statusCode) : _statusCode(statusCode){}

Response::Response(int const & statusCode, std::string const & type) : _statusCode(statusCode), _type(type){}

Response::Response(Response const & other) :
	_statusCode(other._statusCode),
	_type(other._type),
	_body(other._body),
	_statusLine(other._statusLine),
	_header(other._header),
	_fullResponse(other._fullResponse),
	_respCategory(other._respCategory){}

Response &Response::operator=(Response const & other)
{
	if (this != &other)
	{
		_statusCode = other._statusCode;
		_type = other._type;
		_body = other._body;
		_statusLine = other._statusLine;
		_header = other._header;
		_fullResponse = other._fullResponse;
		_respCategory = other._respCategory;
	}
	return *this;
}

Response::~Response(){}

int Response::getStatusCode() const
{
    return _statusCode;
}

void Response::setType(std::string const &type)
{
	_type = type;
}

void Response::setStatusCode(int const &statusCode)
{
	_statusCode = statusCode;
}

void Response::setCategory(int const &category)
{
	_respCategory = category;
}

void Response::addHeader(std::string const &header, std::string const &content)
{
	_header[header] = content;
}

int Response::getRespCategory() const
{
    return _respCategory;
}

std::string Response::getFullResponse()
{
	std::string	headerString;
	std::string	endLine = "\r\n";

	headerString = convertStringMapToString(_header, endLine);
	_fullResponse = _statusLine + endLine + headerString + endLine + _body + endLine;
	return _fullResponse;
}

std::string Response::getBody() const
{
    return _body;
}

void	Response::clearResponse()
{
	_fullResponse.clear();
}

void Response::buildErrorPage(int const & status, t_errPage & errPage, int *fd, std::string & filePath, int & fdType)
{
	stringVectIt	it;

	filePath = ERR_PATH;
	fdType = GET_PHP_SCRIPT;
	if (!errPage.listError.size())
	{
		execCgiDefaultScript(fd);
		return;
	}
	it = std::find(errPage.listError.begin(), errPage.listError.end(), convertIntToString(status));
	if (it != errPage.listError.end())
	{
		fdType = HTML_FILE;
		filePath = errPage.filePath;
		fd[FD_PRINCIPAL] = openFile(filePath);
	}
	else
	{
		execCgiDefaultScript(fd);
	}	
}

void Response::setStatusLine()
{
	_statusLine = "HTTP/1.1 " + convertIntToString(_statusCode) + " " + _allStatusCode[convertIntToString(_statusCode)];
}

void	Response::generateStandardResponse(std::string const &  body)
{
	setStatusLine();
	_body = body;
	_header.clear();
	_header["Content-Length"] = convertIntToString(_body.size() + 2);
	_header["Content-Type"] = _type;
	_header["Connection"] = "keep-alive";
}

void Response::generateChunkedResponse(std::string const & body, int const & status)
{
	setStatusLine();
	_body = body;
	chunkingString(_body, status);
	_header.clear();
	_header["Connection"] = "keep-alive";
	_header["Content-Type"] = _type;
	_header["Transfer-Encoding"] = "chunked";
}

void Response::generateCgiChunkedResponse(std::string const & body, stringMap const & header, int const & status)
{
	setStatusLine();
	_body = body;
	chunkingString(_body, status);
	if (header.size())
	{
		_header.clear();
		_header["Connection"] = "keep-alive";
		_header["Content-Type"] = (*header.find("Content-type")).second;
		_header["Content-Type"] = "text/html; charset=UTF-8";
		_header["Transfer-Encoding"] = "chunked";
	}
}

void Response::generateCatchedResponse(AExcept & e, t_errPage & errPage, int *fd, std::string & filePath, int & fdType)
{
	try
	{
		if (e._type == ERR_RSP)
		{
			ErrorExcept & err = dynamic_cast<ErrorExcept & >(e);
			generateErrorResponse(err, errPage, fd, filePath, fdType);
		}
		else if (e._type == LDIR_RSP)
		{
			LDirExcept & dir = dynamic_cast<LDirExcept & >(e);
			generateLDirResponse(dir);
		}
		else if (e._type == REDIR_RSP)
		{
			RedirExcept & redir = dynamic_cast<RedirExcept & >(e);
			generateRedirectResponse(redir);
		}
	}
	catch(const std::exception& e)
	{
		/*
			What to do when an error occurs when errorPage file is corrupted? Stop the program? [barandri]
		*/
		throw std::runtime_error("AExcept error");
	}
	_respCategory = e._type;
}

void	Response::generateErrorResponse(ErrorExcept & e, t_errPage & errPage, int *fd, std::string & filePath, int & fdType)
{
	_statusCode = e._statusCode;
	setStatusLine();
	buildErrorPage(_statusCode, errPage, fd, filePath, fdType);
}

void Response::generateRedirectResponse(RedirExcept & r)
{
	_statusCode = r._statusCode;
	_type = "text/html";
	setStatusLine();
	_header.clear();
	_body.clear();
	_header["Location"] = r._uri;
	_header["Connection"] = "keep-alive";
	_header["Content-Length"] = "0";
}

void Response::generateLDirResponse(LDirExcept &d)
{
	std::string	list = convertListing(d._dirList);

	_statusCode = d._statusCode;
	_type = "text/html";
	setStatusLine();
	_header.clear();
	_body = LDirExcept::_part[0] + list + LDirExcept::_part[1];
	_header["Content-Length"] = convertIntToString(_body.size());
	_header["Connection"] = "keep-alive";
}

void Response::generateSingleResponse(int const &statusCode)
{
	_statusCode = statusCode;
	_respCategory = SINGLE_RSP;
	_type = "text/html";
	setStatusLine();
	_header.clear();
	_body.clear();
	_header["Connection"] = "keep-alive";
	_header["Content-Length"] = "0";
}

void Response::execCgiDefaultScript(int *fd)
{
	int			pid;
	int			pipeFd[2];
	int			errFd[2];
	CgiParser	cgiParser;
	std::string	statusCodePhrase = _allStatusCode[convertIntToString(_statusCode)];

	cgiParser.configErrEnvp(_statusCode, statusCodePhrase);
	if (pipe(pipeFd) == -1)
		throw std::runtime_error("Pipe fd error");
	if (pipe(errFd) == -1)
		throw std::runtime_error("Pipe err error");
	// std::cout << "Pipe_fd [0] = " << pipeFd[0] << " [1] = " << pipeFd[1] << std::endl;
	// std::cout << "errFd [0] = " << errFd[0] << " [1] = " << errFd[1] << std::endl;
	pid = fork();
	if (pid == -1)
		throw std::runtime_error("Fork error");
	if (pid == 0)
	{
		dup2(errFd[1], STDERR_FILENO);
		close(errFd[0]);
		close(errFd[1]);
		dup2(pipeFd[1], STDOUT_FILENO);
		close(pipeFd[0]);
		close(pipeFd[1]);
		try
		{
			cgiParser.execCgi();
		}
		catch(const std::exception& e)
		{
			fd[FD_PRINCIPAL] = -1;
			fd[FD_STDERR] = -1;
		}
	}
	else
	{
		cgiParser.clearCgiClass();
		close(errFd[1]);
		close(pipeFd[1]);
		fd[FD_PRINCIPAL] = pipeFd[0];
		fd[FD_STDERR] = errFd[0];
	}
}

void Response::initAllStatusCode(std::string const & statPath)
{
	_allStatusCode = splitFileToMapString(statPath, " | ");
}

stringMap Response::getAllStatusCode()
{
    return _allStatusCode;
}
