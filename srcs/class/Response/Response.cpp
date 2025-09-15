/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:41 by barandri          #+#    #+#             */
/*   Updated: 2025/09/15 10:23:52 by rsafidy-         ###   ########.fr       */
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

void Response::setType(std::string const &type)
{
	_type = type;
}

void Response::setStatusCode(int const &statusCode)
{
	_statusCode = statusCode;
}

void Response::setRespCategory(int const &category)
{
	_respCategory = category;
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

void Response::setToDefaultErrorPage()
{
	std::string	replace;

	_body = _defaultErrPage;
	replace = convertIntToString(_statusCode);
	replace += " : " + (*_allStatusCode.find(replace)).second;
	replaceString(_body, replace, "$status");
}

void Response::buildErrorPage(int const & status, t_errPage & errPage)
{
	stringVectIt it;

	if (!errPage.listError.size())
	{
		setToDefaultErrorPage();
		return;
	}
	it = std::find(errPage.listError.begin(), errPage.listError.end(), convertIntToString(status));
	if (it != errPage.listError.end())
		_body = errPage.fileContent;
	else
		setToDefaultErrorPage();
}

void Response::setStatusLine()
{
	_statusLine = "HTTP/1.1 " + convertIntToString(_statusCode) + " " + _allStatusCode[convertIntToString(_statusCode)];
}

void	Response::generateStandardResponse(std::string const &  body)
{
	setStatusLine();
	_body = body;
	_header["Content-Length"] = convertIntToString(_body.size() + 2);
	std::cout << "\n\033[040m aty["<< _type <<"]\033[0m\n" << std::endl;
	_header["Content-Type"] = _type;
	_header["Connection"] = "keep-alive";
}

void Response::generateCatchedResponse(AExcept & e, t_errPage & errPage)
{
	try
	{
		ErrorExcept & err = dynamic_cast<ErrorExcept & >(e);
		generateErrorResponse(err, errPage);
		_respCategory = ERR_RSP;
		return;
	}
	catch(const std::exception & e){}
	try
	{
		RedirExcept & redir = dynamic_cast<RedirExcept & >(e);
		generateRedirectResponse(redir);
		_respCategory = REDIR_RSP;
	}
	catch(const std::exception& e){}
}

void	Response::generateErrorResponse(ErrorExcept & e, t_errPage & errPage)
{
	_statusCode = e._statusCode;
	setStatusLine();
	buildErrorPage(_statusCode, errPage);
	_header["Content-Length"] = convertIntToString(_body.size() + 2);
	_header["Content-Type"] = "text/html";
	_header["Connection"] = "keep-alive";
}

void Response::generateRedirectResponse(RedirExcept & r)
{
	_statusCode = r._statusCode;
	setStatusLine();
	_header["Location"] = r._uri;
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
