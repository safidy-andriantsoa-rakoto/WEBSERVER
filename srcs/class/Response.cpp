/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:41 by barandri          #+#    #+#             */
/*   Updated: 2025/07/30 09:02:42 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

stringMap Response::_allStatusCode;

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

int	Response::getStatusCode() const
{
	return _statusCode;
}

void Response::setStatusCode(int const &statusCode)
{
	_statusCode = statusCode;
}

void Response::setType(std::string const &type)
{
	_type = type;
}

// void	Response::setResponseBody(std::string const & path)
// {
// 	_body = readFileToString(path);
// }

std::string Response::getStatusLine(int const &statusCode)
{
	std::string	statusLine;
	std::string	reasonPhrase;
	std::string	statusCodeString;

	statusCodeString = convertIntToString(statusCode);
	statusLine = "HTTP/1.1 " + statusCodeString + " " + reasonPhrase;
	reasonPhrase = (*_allStatusCode.find(statusCodeString)).second;
    return statusLine;
}

std::string Response::responseGenerator(std::string const &responsePath)
{
	std::string	fullResponse;
	std::string	statusLine;
	std::string	header;
	std::string	body;
	std::string	endLine = "\r\n";

	body = readFileToString(responsePath);
	statusLine = getStatusLine(_statusCode) + endLine;
	header = "Content-Type: " + _type + endLine + "Content-Length: " + convertIntToString(body.size()) + endLine;
	fullResponse = statusLine + header + endLine + body + endLine;
	return fullResponse;
}

// Use a generic function to replace a word inside a file ($env)
// Read the file and add endline correctly
std::string Response::responseGenerator(int const & statusCode, std::string const & responsePath, std::string const & type)
{
	std::string	fullResponse;
	std::string	statusLine;
	std::string	header;
	std::string	body;
	std::string	endLine = "\r\n";

	body = readFileToString(responsePath);
	statusLine = getStatusLine(statusCode) + endLine;
	header = "Content-Type: " + type + endLine + "Content-Length: " + convertIntToString(body.size()) + endLine;
	fullResponse = statusLine + header + endLine + body + endLine;
	return fullResponse;
}

void Response::initAllStatusCode(std::string const & filePath)
{
	_allStatusCode = splitFileToMapString(filePath, " | ");
}
