/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:36 by barandri          #+#    #+#             */
/*   Updated: 2025/07/28 09:16:26 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(){}

Request::Request(Request const & other)
{
	*this = other;
}

Request &Request::operator=(Request const & other)
{
	if (this != &other)
	{
		_requestLine = other._requestLine;
		_header = other._header;
		_body = other._body;
	}
	return *this;
}

Request::~Request(){}

std::string Request::getRequestLine() const
{
	return _requestLine;
}

std::string Request::getMethod() const
{
    return _method;
}

std::string Request::getRequestTarget() const
{
    return _requestTarget;
}

std::string Request::getHttpVersion() const
{
    return _clientHttpVersion;
}

stringMap Request::getHeader() const
{
	return _header;
}

std::string Request::getBody() const
{
	return _body;
}

void Request::setRequestLine(std::string & replacement)
{
	stringVect	splitted;

	_requestLine = replacement;
	splitted = splitString(replacement, " ");
	if (splitted.size() != 3)
		throw std::runtime_error("Error on request line");
	_method = splitted[0];
	_requestTarget = splitted[1];
	_clientHttpVersion = splitted[2];
}

void Request::setBody(std::string & replacement)
{
	_body = replacement;
}

void Request::addHeader(std::string & field, std::string & content)
{
	stringPair pair(field, content);

	_header.insert(pair);
}

void Request::displayRequest() const
{
	std::cout << "==================REQUEST PARSING==================" << std::endl
	<< "==== Request Line ===" << std::endl;
	std::cout << "Method = ["<< _method << "]" << std::endl
	<< "Request target = [" << _requestTarget << "]" << std::endl
	<< "HTTP = [" << _clientHttpVersion << "]" << std::endl
	<< "==== Header field ===" << std::endl;
	displayStringMap(_header);
	if (_body.size())
		std::cout << std::endl << "==== Body ===" << std::endl << _body << std::endl;
	std::cout << "==============END OF REQUEST PARSING=============" << std::endl;
}
