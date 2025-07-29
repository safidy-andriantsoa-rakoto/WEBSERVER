/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:41 by barandri          #+#    #+#             */
/*   Updated: 2025/07/29 11:02:36 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(){}

Response::Response(Response const & other)
{
	*this = other;
}

Response &Response::operator=(Response const & other)
{
	if (this != &other)
	{
		_statusLine = other._statusLine;
		_statusCode = other._statusCode;
		_header = other._header;
		_body = other._body;
	}
	return *this;
}

Response::~Response(){}

int	Response::getStatusCode() const
{
	return _statusCode;
}

std::string	Response::getResponseFile() const
{
	return _body;
}

void	Response::setStatusLine(int const & status, std::string const & reasonPhrase)
{
	std::stringstream	ss;
	std::string			statusCodeString;

	_statusCode = status;
	ss << status;
	ss >> statusCodeString;
	_statusLine = "HTTP/1.1 " + statusCodeString + " " + reasonPhrase;  
}

void	Response::setResponseBody(std::string const & path)
{
	_body = readFileToString(path);
}

void	Response::addHeader(std::string const & field, std::string const & content)
{
	stringPair pair(field, content);

	_header.insert(pair);
}

void	Response::addHeader(std::string const & field, int const & content)
{
	std::stringstream	ss;
	std::string			add;
	ss << content;
	ss >> add;
	stringPair pair(field, add);
	_header.insert(pair);
}

std::string	Response::joinHeader() const
{
	std::string	result;
	stringMapIt	it;

	for (it = _header.begin(); it != _header.end(); it++)
		result += (*it).first + ": " + (*it).second + "\n";
	return result;
}

std::string Response::responseGenerator()
{
	std::string			fullheader;
	std::string			fullResponse;
	std::string			endLine = "\r\n";

	fullheader = joinHeader();
	fullResponse = _statusLine + endLine + fullheader + endLine + _body + endLine;
	std::cout << "=========RESPONSE=========\n" << fullResponse << std::endl;
	return fullResponse;
}
