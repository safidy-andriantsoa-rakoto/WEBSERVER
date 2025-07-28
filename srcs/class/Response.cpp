/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:41 by barandri          #+#    #+#             */
/*   Updated: 2025/07/28 09:28:32 by barandri         ###   ########.fr       */
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
		_statusCode = other._statusCode;
		_body = other._body;
	}
	return *this;
}

Response::~Response(){}

int Response::getStatusCode() const
{
	return _statusCode;
}

std::string Response::getResponseFile() const
{
	return _body;
}

void Response::setStatusCode(int const & status)
{
	_statusCode = status;
}

void Response::setResponseFile(std::string const & path)
{
	_body = readFileToString(path);
}

//Review HTTP rule to generate response
std::string Response::responseGenerator() const
{
	std::string			fullResponse;
	std::string			status;
	std::stringstream	ss;

	ss << _statusCode;
	ss >> status;
	fullResponse =  "HTTP/1.1 " + status + "\r\n" + _body;
	return fullResponse;
}
