/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:36 by barandri          #+#    #+#             */
/*   Updated: 2025/09/30 11:04:35 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(): _request_length(0), _autoIdx(false), _is_CRLF_header(false), _isCgi(false) {}

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
		_isCgi = other._isCgi;
		_cgi = other._cgi;
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

bool Request::getIsCgi() const
{
    return _isCgi;
}

void Request::setIsCgi(bool const &condition)
{
	_isCgi = condition;
}

Cgi Request::getCgiClass() const
{
    return _cgi;
}

void Request::processCgiClass(std::string const &target, std::string const & rootDirectory)
{
	_cgi.parseCgiEnv(target, rootDirectory);
	_cgi.setReqMethod(_method);
	_cgi.setContentLength(_request_length);
	_cgi.displayCgiClass(); //For test only
	_cgi.configEnvp();
}

void Request::setCgiClass(Cgi const &cgi)
{
	_cgi = cgi;
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

void Request::set_Body(const std::string& _body)
{
	this->_body.clear();
	if (!_body.size())
		this->_body = "";
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
	//Length error management to see with POST method
	_body = content;
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
	setRequestBody(requeststring, pos + sep.size());
	if (requeststring.find(sep + sep) != std::string::npos && _method == "POST")
		this->_is_CRLF_header = true;
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
