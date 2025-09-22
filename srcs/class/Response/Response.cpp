/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:41 by barandri          #+#    #+#             */
/*   Updated: 2025/09/22 08:36:44 by barandri         ###   ########.fr       */
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

// void Response::setToDefaultErrorPage()
// {
// 	std::string	replace;

// 	_body = _defaultErrPage;
// 	replace = convertIntToString(_statusCode);
// 	replace += " : " + (*_allStatusCode.find(replace)).second;
// 	replaceString(_body, replace, "$status");
// }

void Response::buildErrorPage(int const & status, t_errPage & errPage, int & fd, std::string & filePath)
{
	stringVectIt it;

	std::cout << "Build error page\n";
	filePath = ERR_PATH;
	if (!errPage.listError.size())
	{
		fd = openFile(filePath);
		std::cout << "Default Error page fd = " << fd << std::endl;
		return;
	}
	it = std::find(errPage.listError.begin(), errPage.listError.end(), convertIntToString(status));
	if (it != errPage.listError.end())
	{
		filePath = errPage.filePath;
		fd = openFile(errPage.filePath);
	}
	else
		fd = openFile(filePath);
	std::cout << "Error page fd = " << fd << std::endl;
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

void Response::generateCatchedResponse(AExcept & e, t_errPage & errPage, int & fd, std::string & filePath)
{
	std::cout << "Generate catched response\n";
	try
	{
		if (e._type == ERR_RSP)
		{
			ErrorExcept & err = dynamic_cast<ErrorExcept & >(e);
			generateErrorResponse(err, errPage, fd, filePath);
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
		std::cerr << "AExcept casting error : " << e.what() << std::endl;
	}
	_respCategory = e._type;
}

void	Response::generateErrorResponse(ErrorExcept & e, t_errPage & errPage, int & fd, std::string & filePath)
{
	std::cout << "Generate error response\n";

	_statusCode = e._statusCode;
	setStatusLine();
	buildErrorPage(_statusCode, errPage, fd, filePath);
	// _header["Content-Length"] = convertIntToString(_body.size() + 2);
	// _header["Content-Type"] = "text/html";
	// _header["Connection"] = "keep-alive";
}

void Response::generateRedirectResponse(RedirExcept & r)
{
	std::cout << "Generate redirection response\n";
	_statusCode = r._statusCode;
	_type = "text/html"; //A voir
	setStatusLine();
	_header["Location"] = r._uri;
	_header["Connection"] = "keep-alive";
	_header["Content-Length"] = "0";
}

void Response::generateLDirResponse(LDirExcept &d)
{
	_statusCode = d._statusCode;
	setStatusLine();
	_header["Connection"] = "keep-alive";
}

void Response::initResponseUtils(std::string const & statPath, std::string const & errPath)
{
	_allStatusCode = splitFileToMapString(statPath, " | ");
	_defaultErrPage = readFileToString(errPath);
}

stringMap Response::getAllStatusCode()
{
    return _allStatusCode;
}
