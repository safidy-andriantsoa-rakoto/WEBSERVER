/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:41 by barandri          #+#    #+#             */
/*   Updated: 2025/07/29 14:46:02 by barandri         ###   ########.fr       */
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
		_statusCode = other._statusCode;
		_body = other._body;
	}
	return *this;
}

Response::~Response(){}

int	Response::getStatusCode() const
{
	return _statusCode;
}

void	Response::setResponseBody(std::string const & path)
{
	_body = readFileToString(path);
}

//Use a generic function to replace a word inside a file ($env)
//Read the file and add endline correctly
std::string Response::responseGenerator(std::string const & responsePath, int const & statusCode)
{
	std::string			fullResponse;
	std::string			endLine = "\r\n";

	return fullResponse;
}

void Response::initAllStatusCode(std::string const & filePath)
{
	_allStatusCode = splitFileToMapString(filePath, " | ");
}
