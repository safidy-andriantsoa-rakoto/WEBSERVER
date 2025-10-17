/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:41 by barandri          #+#    #+#             */
/*   Updated: 2025/10/17 09:46:04 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

stringMap	Response::_allStatusCode;

Response::Response() : _respCategory(STD_RSP), _pos(0){}

Response::Response(int const & statusCode) : _statusCode(statusCode), _pos(0){}

Response::Response(int const & statusCode, std::string const & type) : _statusCode(statusCode), _type(type), _pos(0){}

Response::Response(Response const & other) :
	_statusCode(other._statusCode),
	_type(other._type),
	_body(other._body),
	_statusLine(other._statusLine),
	_header(other._header),
	_fullResponse(other._fullResponse),
	_respCategory(other._respCategory),
	_respStat(other._respStat),
	_originPath(other._originPath),
	_originBody(other._originBody),
	_pos(other._pos){}

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
		_respStat = other._respStat;
		_originPath = other._originPath;
		_originBody = other._originBody;
		_pos = other._pos;
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

std::string Response::getType() const
{
    return _type;
}

void Response::setStatusCode(int const &statusCode)
{
	_statusCode = statusCode;
}

void Response::setRespCategory(int const &category)
{
	_respCategory = category;
}

size_t Response::getPosition() const
{
    return _pos;
}

void Response::setPosition(size_t const &newPos)
{
	_pos = newPos;
}

int Response::getRespStat() const
{
    return _respStat;
}

void Response::setRespStat(int const &stat)
{
	_respStat = stat;
}

std::string Response::getOriginPath() const
{
    return _originPath;
}

void Response::setOriginPath(std::string const &path)
{
	_originPath = path;
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

std::string Response::getOriginBody() const
{
    return _originBody;
}

void Response::setOriginBody(std::string const &body)
{
	_originBody = body;
}

void	Response::clearResponse()
{
	_fullResponse.clear();
}


void Response::buildErrorPage(int const & status, t_errPage & errPage, std::string & filePath, int & fdType)
{
	stringVectIt	it;
	std::string		content;

	filePath = ERR_PATH;
	fdType = STD_FILE;
	if (errPage.listError.size())
	{
		content = readFileToString(filePath);
		it = std::find(errPage.listError.begin(), errPage.listError.end(), convertIntToString(status));
		if (it != errPage.listError.end())
			filePath = errPage.filePath;
	}
	try
	{
		content = readFileToString(filePath);
		if (content.size() <= _BUFFER)
		{
			_body = content;
			_respStat = OP_ONCE;
		}
		else
		{
			_originBody = content;
			_respStat = OP_START;
		}
		// _pos = 0;
		_originPath = filePath;
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error("ERROR PAGE NOT FOUND");
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

void Response::generateChunkedResponse(std::string const & body)
{
	setStatusLine();
	_body = body;
	chunkingString(_body, _respStat);
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
		_header["Transfer-Encoding"] = "chunked";
	}
}

void Response::generateCatchedResponse(AExcept & e, t_errPage & errPage, std::string & filePath, int & fdType)
{
	try
	{
		if (e._type == ERR_RSP)
		{
			ErrorExcept & err = dynamic_cast<ErrorExcept & >(e);
			generateErrorResponse(err, errPage, filePath, fdType);
		}
		else if (e._type == LDIR_RSP)
		{
			LDirExcept & dir = dynamic_cast<LDirExcept & >(e);
			generateLDirResponse(dir);
			_respStat = OP_ONCE;
		}
		else if (e._type == REDIR_RSP)
		{
			RedirExcept & redir = dynamic_cast<RedirExcept & >(e);
			generateRedirectResponse(redir);
			_respStat = OP_ONCE;
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

void	Response::generateErrorResponse(ErrorExcept & e, t_errPage & errPage, std::string & filePath, int & fdType)
{
	_statusCode = e._statusCode;
	setStatusLine();
	buildErrorPage(_statusCode, errPage, filePath, fdType);
	_type = Mime::getFileType(_originPath);
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
	_respStat = OP_ONCE;
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
			fd[FD_STDOUT] = -1;
			fd[FD_STDERR] = -1;
		}
	}
	else
	{
		cgiParser.clearCgiClass();
		close(errFd[1]);
		close(pipeFd[1]);
		fd[FD_STDOUT] = pipeFd[0];
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
