/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:36 by barandri          #+#    #+#             */
/*   Updated: 2025/09/24 08:56:19 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(): _request_length(0), _autoIdx(false) {}

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
		_request_length = other._request_length;
		_autoIdx = other._autoIdx;
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

std::string Request::getHttp() const
{
    return _clientHttp;
}

stringMap Request::getHeader() const
{
	return _header;
}

const MapSVectStr& Request::getpostMultipartData() const
{
    return (this->_multipart_body);
}

void Request::Del_space(std::string &data)
{
    while (-1 != static_cast<int>(data.find(' ')))
        data.erase(data.find(' '), 1);
}

bool Request::getAutoIdx() const
{
    return _autoIdx;
}

void Request::setAutoIdx(bool const &autoIdx)
{
	_autoIdx = autoIdx;
}

void Request::setRequestSize(ssize_t size)
{
	this->_request_length = size;
}

ssize_t Request::getRequestSize()
{
	return (this->_request_length);
}

void Request::set_post_body(stringVect var, std::string content)
{
	if (var.size())
		_multipart_body[var] = content;
}

std::string Request::getBody() const
{
	return _body;
}

void Request::setRequestTarget(std::string const &target)
{
	_requestTarget = target;
}

void Request::setRequestLine(std::string const & replacement)
{
	stringVect	splitted;

	_requestLine = replacement;
	splitted = splitString(replacement, " ");
	if (splitted.size() != 3)
	{
		std::cout << "\033[42mRequest = \n" << _requestLine << "\033[0m" << std::endl;
		throw std::runtime_error("Error on request line");
	}
	_method = splitted[0];
	size_t f = splitted[1].find_first_of('?');
	if (f != std::string::npos)
		_requestTarget =  splitted[1].substr(0, f);
	else
		_requestTarget = splitted[1];
	_clientHttp = splitted[2];
}

void	Request::setRequestBody(std::string const & requeststring, size_t const & pos)
{
	int			length;
	std::string	content;
	stringMapIt	it;

	length = 0;
	it = _header.find("Content-Length");
	if (it == _header.end())
		return ;
	length = atoi((*it).second.c_str());
	content = requeststring.substr(pos, static_cast<size_t>(length));
	//Length error management to see
	_body = content;
}

void	Request::parseRequest(std::string const & requeststring)
{
	size_t		pos;
	size_t		startPos;
	std::string	sep = "\r\n";
	std::string	line;
	// std::string key = "Content-Disposition";
	// std::string type = "name=\"";
	// std::string name = "filename=\"";
	stringVect	headerVect;
//Transfer-Encoding

	// std::cout << "REQUEST["<< requeststring <<"]" << std::endl;
	startPos = 0;
	pos = requeststring.find(sep, startPos);
	if (pos == std::string::npos)
		throw std::runtime_error("Parsing request : No request line found");
	line = requeststring.substr(startPos, pos - startPos);
	setRequestLine(line);
	startPos = pos + sep.size();
	while (pos != std::string::npos)
	{
		pos = requeststring.find(sep, startPos);
		if (pos == startPos)
			break ;
		line = requeststring.substr(startPos, pos - startPos);
		headerVect = strictSplit(line, ": ");
		addToMapString(_header, headerVect[0], headerVect[1]);
		startPos = pos + sep.size();
		headerVect.clear();
	}
	setRequestBody(requeststring, pos + sep.size());
	// displayRequest();
	// std::cout << "[ "<< _body <<" ]" << std::endl;
	// pos = _body.find("\n" + sep);
	// if (pos != std::string::npos)
	// {
	// 	std::cout << "POST FILE" << std::endl;
		// std::string _postNextHead = _body.substr(0, pos);
		// this->Del_space(_postNextHead);
		// pos = _postNextHead.find(type);
		// if (pos != std::string::npos)
		// {
		// 	std::string tmp = _postNextHead.substr(pos + type.size(), _postNextHead.size());
		// 	size_t end = tmp.find_first_of('"');
		// 	if (end != std::string::npos)
		// 		std::cout << tmp.substr(0, end) << std::endl;
		// 	_postInfo["type"] =  _postNextHead.erase(0, pos);
		// }
		// pos = _postNextHead.find(name);
		// if (pos != std::string::npos)
		// {
		// 	std::string tmp = _postNextHead.substr(pos + name.size(), _postNextHead.size());
		// 	size_t end = tmp.find_first_of('"');
		// 	if (end != std::string::npos)
		// 		std::cout << tmp.substr(0, end) << std::endl;
		// 	_postInfo["name"] = _postNextHead.erase(0, pos);
		// }
		// pos = _body.find_first_of("\n\r\n");
		// if (pos != std::string::npos)
		// {
		// 	_body = _body.erase(0, pos);
		// 	// std::cout << "BODY>>[" << _body << std::endl;
		// }
	// }
	// else
		// std::cout << "FORMULAIRE REQUEST POST" << std::endl;
}

void Request::displayRequest() const
{
	std::cout << "\033[44m==================REQUEST PARSING==================" << std::endl
	<< "==== Request Line ===" << std::endl;
	std::cout << "Method = ["<< _method << "]" << std::endl
	<< "Request target = [" << _requestTarget << "]" << std::endl
	<< "HTTP = [" << _clientHttp << "]" << std::endl
	<< "==== Header field ===" << std::endl;
	displayStringMap(_header);
	if (_body.size())
		std::cout << std::endl << "==== Body ===" << std::endl << _body << std::endl;
	std::cout << "==============END OF REQUEST PARSING=============\033[0m" << std::endl;
}

void Request::displayRequestMultipartData()
{

	std::cout << "==============================MULTIPARTDATA================================" << std::endl;
	for (MapSVectStrIt it = _multipart_body.begin(); it != _multipart_body.end(); it++)
	{
		std::cout << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << std::endl;
		stringVect tmp = it->first;
		for (stringVectIt its = tmp.begin(); its != tmp.end(); its++)
		{
			std::cout << "	\033[36m[" << *its << "]\033[0m" << std::endl;
		}
		std::cout << "----------------------------------------------------------------------------" << std::endl;
		std::cout << "			\033[32m->" << it->second << "<-\033[0m" << std::endl;
		std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
		
	}
	std::cout << "==============================MULTIPARTDATA END================================" << std::endl;
	
}
