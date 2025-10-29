/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:36 by barandri          #+#    #+#             */
/*   Updated: 2025/10/29 13:51:40 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(): _request_length(0),
	_autoIdx(false),
	_isChunked(false),
	_isEOF(false),
	_content_length(0),
	_c_length_received(0),
	_isCgi(false),
	_is_CRLF_header(false){}

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
		_isChunked = other._isChunked;
		_isEOF = other._isEOF;
		_multipart_body = other._multipart_body;
		_content_length = other._content_length;
		_c_length_received = other._c_length_received;
		_isCgi = other._isCgi;
		_cgiParser = other._cgiParser;
		_is_CRLF_header = other._is_CRLF_header;
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

bool Request::getIsChunked() const
{
    return _isChunked;
}

void Request::setIsChunked(bool const &stat)
{
	_isChunked = stat;
}

bool Request::getIsEOF() const
{
    return _isEOF;
}

void Request::setIsEOF(bool const &stat)
{
	_isEOF = stat;
}

CgiParser Request::getCgiParser() const
{
    return _cgiParser;
}

void Request::processCgiClass(std::string & target, std::string const & execPath)
{
	clearCgiClass();
	setIsCgi(true);
	_cgiParser.parseCgiEnv(target);
	_cgiParser.setReqMethod(_method);
	_cgiParser.setContentLength(_body.size());
	_cgiParser.setCgiExecPath(execPath);
	stringMapIt it = _header.find("Content-Type");
	if (it != _header.end())
		_cgiParser.setContentType((*it).second);
	_cgiParser.buildEnvp();
	target = _cgiParser.getScriptFileName();
}

void Request::setCgiClass(CgiParser const &cgi)
{
	_cgiParser = cgi;
}

void Request::execCgiClass()
{
	_cgiParser.execCgi();
}

void Request::clearCgiClass()
{
	_cgiParser.clearCgiClass();
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

void Request::clear_body()
{
	this->_body.clear();
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

void	Request::addRequestBody(std::string const & requeststring, size_t const & pos)
{
	int			length;
	std::string	content;
	stringMapIt	it;

	if (_isChunked == true)
	{
		// size_t posEnd = requeststring.find_last_of("\r\n");
		length = requeststring.size() - pos;
	}
	else
	{
		length = 0;
		it = _header.find("Content-Length");
		if (_isChunked ==false && it == _header.end())
			return ;
		length = atoi((*it).second.c_str());
	}
	content = requeststring.substr(pos, static_cast<size_t>(length));
	_body = content;
	_c_length_received = content.size();
	_content_length = length;
	// std::cout << "\nBODY=\n" << _body << "\n"<< std::endl;
	std::cout << "\nCONTENT_LENGTH=" << _content_length << " \nreceived =" << content.size() << "\n"<< std::endl;
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
	addRequestBody(requeststring, pos + sep.size());
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

size_t Request::get_c_length_received()
{
    return (this->_c_length_received);
}

void Request::set_c_length_received(size_t received)
{
	if ((int)received > -1)
		this->_c_length_received += received;
	std::cout << "			ACTUAL RECEIVED === " << _c_length_received << std::endl;
}

size_t Request::get_content_length()
{
    return (this->_content_length);
}

void Request::set_content_length(size_t size)
{
	this->_content_length = size;
}

bool Request::checkContentReceived() const
{
	if (_c_length_received != _content_length)
    	return false;
	return true;
}

void Request::delete_range_body_request(int start, int end)
{
	if (_body.size() && start > -1 && end > -1 && end > start)
	{
		if ((size_t)(end) > _body.size())
			_body.erase(_body.begin() + start, _body.end());
		else
			_body.erase(_body.begin() + start, _body.begin() + end);
	}
}


void Request::set_is_CRLF_header(bool is_crlf)
{
	this->_is_CRLF_header = is_crlf;
}

bool Request::get_is_CRLF_header()
{
    return (this->_is_CRLF_header);
}