/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:36 by barandri          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/09/20 21:08:26 by rsafidy-         ###   ########.fr       */
=======
/*   Updated: 2025/10/02 11:53:48 by rsafidy-         ###   ########.fr       */
>>>>>>> 6ae73645f3cc8449709d44042828db1faf1e59a2
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(): _request_length(0), _autoIdx(false), _is_CRLF_header(false), _content_length(0), _c_length_received(0)
{}

Request::Request(Request const & other)
{
	*this = other;
}

Request &Request::operator=(Request const & other)
{
	if (this != &other)
	{
		_method = other._method;
		_requestTarget = other._requestTarget;
		_originalRequestTarget = other._originalRequestTarget;
		_clientHttp = other._clientHttp;
		_requestLine = other._requestLine;
		_header = other._header;
		_body = other._body;
		_request_length = other._request_length;
		_autoIdx = other._autoIdx;
		_is_CRLF_header = other._is_CRLF_header;
		_multipart_body = other._multipart_body;
		_content_length = other._content_length;
		_c_length_received = other._c_length_received;
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

std::string Request::getOriginalRequestTarget() const
{
    return _originalRequestTarget;
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

void Request::set_is_CRLF_header(bool is_crlf)
{
	this->_is_CRLF_header = is_crlf;
}

bool Request::get_is_CRLF_header()
{
    return (this->_is_CRLF_header);
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

<<<<<<< HEAD
std::string& Request::getBody()
=======
void Request::set_Body(const std::string& body)
{
	this->_body.clear();
	if (!body.size())
		this->_body = "";
	else
		this->_body = body;
}

std::string& Request::get_Body()
{
    return (_body);
}

void Request::set_post_body(stringVect var, std::string content)
{
	if (var.size())
		_multipart_body[var] = content;
}

std::string Request::getBody() const
>>>>>>> 6ae73645f3cc8449709d44042828db1faf1e59a2
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
	_originalRequestTarget = _requestTarget;
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
	_body = content;//alana @zay eee
	_c_length_received += content.size();
	_content_length = length;
	
	std::cout << "\nCONTENT_LENGTH=" << _content_length << " received =" << content.size() << "\n"<< std::endl;
}

void	Request::parseRequest(std::string const & requeststring)
{
	size_t		pos;
	size_t		startPos;
	std::string	sep = "\r\n";
	std::string	line;
	stringVect	headerVect;

	startPos = 0;
	// std::cout << "REQUEST_STRING--->" << requeststring << "<----" << std::endl;
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
<<<<<<< HEAD
	// setRequestBody(requeststring, pos + sep.size());
	// std::cout << this->_body << std::endl;
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
=======
	setRequestBody(requeststring, pos + sep.size());
	if (requeststring.find(sep + sep) != std::string::npos && _method == "POST")
		this->_is_CRLF_header = true;
	// if (this->_body_two.size())
	// 	std::cout << "[" << _body_two[0] << "]" << std::endl;
>>>>>>> 6ae73645f3cc8449709d44042828db1faf1e59a2
	// else
	// 	std::cout << ""
	
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

size_t Request::get_c_length_received()
{
    return (this->_c_length_received);
}

void Request::set_c_length_received(size_t received)
{
	if ((int)received > -1)
		this->_c_length_received += received;
}

size_t Request::get_content_length()
{
    return (this->_content_length);
}
