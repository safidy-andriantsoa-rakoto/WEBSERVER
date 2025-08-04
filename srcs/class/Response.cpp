/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:41 by barandri          #+#    #+#             */
/*   Updated: 2025/08/04 16:01:25 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

stringMap Response::_allStatusCode;

void Response::addDefaultHeader()
{
}

Response::Response(){}

Response::Response(Response const & other)
{
	*this = other;
}

Response &Response::operator=(Response const & other)
{
	if (this != &other)
	{
		// _statusCode = other._statusCode;
		// _body = other._body;
	}
	return *this;
}

Response::~Response(){}

std::string Response::getFullResponse()
{
	return _fullResponse;
}

Response::Response(int const &statusCode)
{
	std::string	statusLine;
	std::string	reasonPhrase;
	std::string	statusCodeString;

	statusCodeString = convertIntToString(statusCode);
	reasonPhrase = (*_allStatusCode.find(statusCodeString)).second;
	statusLine = "HTTP/1.1 " + statusCodeString + " " + reasonPhrase;
}

Response::Response(std::string const &responsePath)
{
	std::string	fullResponse;
	std::string	statusLine;
	std::string	header;
	std::string	body;
	std::string	endLine = "\r\n";

	body = readFileToString(responsePath);
	statusLine = convertIntToString(_statusCode) + _allStatusCode[convertIntToString(_statusCode)] + endLine;
	header = "Content-Type: " + _type + endLine + "Content-Length: " + convertIntToString(body.size()) + endLine;
	fullResponse = statusLine + header + endLine + body + endLine;
}

Response::Response(int const & statusCode, std::string const &responsePath)
{
	std::string	fullResponse;
	std::string	statusLine;
	std::string	header;
	std::string	body;
	std::string	replace;
	std::string	endLine = "\r\n";

	body = readFileToString(responsePath);
	replace = convertIntToString(statusCode);
	replace += " : " + (*_allStatusCode.find(replace)).second;
	replaceString(body, replace, "$status");
	statusLine = convertIntToString(_statusCode) + _allStatusCode[convertIntToString(_statusCode)] + endLine;
	header = "Content-Type: " + _type + endLine + "Content-Length: " + convertIntToString(body.size()) + endLine;
	_fullResponse = statusLine + header + endLine + body + endLine;
}

Response::Response(int const & statusCode, std::string const & responsePath, std::string const & type)
{
	std::string	fullResponse;
	std::string	statusLine;
	std::string	headerString;
	std::string	endLine = "\r\n";

	_body = readFileToString(responsePath);
	statusLine = convertIntToString(_statusCode) + _allStatusCode[convertIntToString(_statusCode)] + endLine;
	headerString = "Content-Type: " + type + endLine + "Content-Length: " + convertIntToString(_body.size()) + endLine;
	_fullResponse = statusLine + headerString + endLine + _body + endLine;
}

void Response::initAllStatusCode(std::string const & filePath)
{
	_allStatusCode = splitFileToMapString(filePath, " | ");
}
