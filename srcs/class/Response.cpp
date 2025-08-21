/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:41 by barandri          #+#    #+#             */
/*   Updated: 2025/08/21 09:29:32 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

stringMap	Response::_allStatusCode;
std::string	Response::_defaultErrPage;

void Response::addDefaultHeader(){}

Response::Response(){}

Response::Response(int const & statusCode) : _statusCode(statusCode){}

Response::Response(int const & statusCode, std::string const & type) : _statusCode(statusCode), _type(type){}

Response::Response(Response const & other)
{
	*this = other;
}

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
	}
	return *this;
}

Response::~Response(){}

std::string Response::getFullResponse()
{
	return _fullResponse;
}

void Response::buildErrorPage()
{
	std::string	replace;

	//Check from config errPage
	//If statusCode find inside config with another err page => _body = errPage(content)
	//Else _body = _defaultErrPage;
	_body = _defaultErrPage;
	replace = convertIntToString(_statusCode);
	replace += " : " + (*_allStatusCode.find(replace)).second;
	replaceString(_body, replace, "$status");
}

void Response::setStatusLine()
{
	_statusLine = "HTTP/1.0 " + convertIntToString(_statusCode) + " " + _allStatusCode[convertIntToString(_statusCode)];
}

void	Response::responseGenerator(std::string const &  filePath)
{
	std::string	headerString;
	std::string	endLine = "\r\n";

	setStatusLine();
	_body = readFileToString(filePath);
	_header["Content-Length"] = convertIntToString(_body.size());
	headerString = convertStringMapToString(_header, endLine);
	_fullResponse = _statusLine + endLine + headerString + endLine + _body + endLine;
}

//Error response + replacing html page
void	Response::responseErrorGenerator(int const & statusCode)
{
	std::string	headerString;
	std::string	replace;
	std::string	endLine = "\r\n";

	_statusCode = statusCode;
	setStatusLine();
	buildErrorPage();
	_header["Content-Length"] = convertIntToString(_body.size());
	headerString = convertStringMapToString(_header, endLine);
	_fullResponse = _statusLine + endLine + headerString + endLine + _body + endLine;
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
