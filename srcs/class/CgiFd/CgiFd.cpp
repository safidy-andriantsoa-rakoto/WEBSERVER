/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiFd.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:36 by barandri          #+#    #+#             */
/*   Updated: 2025/10/01 19:37:44 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiFd.hpp"

CgiFd::CgiFd(){}

CgiFd::CgiFd(int const &CgiFd, int const &clientFd) : 
	_outputFd(CgiFd),
	_clientFd(clientFd),
	_status(OP_OPEN)
	{
		_isError = false;
		_isHeader = false;
		_statusCode = -1;
	}

CgiFd::CgiFd(int const &outputFd, int const & errorFd, int const & inputFd, int const &clientFd) : 
	_outputFd(outputFd),
	_errorFd(errorFd),
	_inputFd(inputFd),
	_clientFd(clientFd),
	_status(OP_OPEN)
	{
		_isError = false;
		_isHeader = false;
		_statusCode = -1;
	}

CgiFd::CgiFd(CgiFd const & other)
{
	*this = other;
}

CgiFd &CgiFd::operator=(CgiFd const & other)
{
	if (this != &other)
	{
		_clientFd = other._clientFd;
		_outputFd = other._outputFd;
		_errorFd = other._errorFd;
		_inputFd = other._inputFd;
		_buffer = other._buffer;
		_status = other._status;
		_isError = other._isError;
		_statusCode = other._statusCode;
		_isHeader = other._isHeader;
		_header = other._header;
		_body = other._body;
	}
	return *this;
}

CgiFd::~CgiFd(){}

int CgiFd::getOutputFd() const
{
    return _outputFd;
}

void CgiFd::setOutputFd(int const &fd)
{
	_outputFd = fd;
}

int CgiFd::getErrFd() const
{
    return _errorFd;
}

int CgiFd::getInputFd() const
{
    return _inputFd;
}

int CgiFd::getClientFd() const
{
    return _clientFd;
}

std::string CgiFd::getBuffer() const
{
    return _buffer;
}

int CgiFd::getStatus() const
{
    return _status;
}

void CgiFd::setStatus(int const &status)
{
	_status = status;
}

bool CgiFd::getIsError() const
{
    return _isError;
}

void CgiFd::setIsError(bool const &stat)
{
	_isError = stat;
}

void CgiFd::setBuffer(std::string const &content)
{
	_buffer = content;
}

void CgiFd::bufferParser()
{
	size_t		pos;
	size_t		startPos;
	std::string	sep = "\r\n";
	std::string	line;
	stringVect	headerVect;

	_header.clear();
	_body.clear();
	if (_isHeader == true)
	{
		_body = _buffer;
		return;
	}
	startPos = 0;
	pos = 0;
	while (pos != std::string::npos)
	{
		pos = _buffer.find(sep, startPos);
		if (pos == startPos)
			break ;
		line = _buffer.substr(startPos, pos - startPos);
		headerVect = strictSplit(line, ": ");
		addToMapString(_header, headerVect[0], headerVect[1]);
		startPos = pos + sep.size();
		headerVect.clear();
	}
	_isHeader = true;
	_body = _buffer.substr(pos, _buffer.size() - pos);
}

void CgiFd::clearBuffer()
{
	_buffer.clear();
	_body.clear();
	_header.clear();
}

std::string CgiFd::getBody() const
{
    return _body;
}

stringMap CgiFd::getHeader() const
{
    return _header;
}

int CgiFd::getStatusCode()
{
    return _statusCode;
}

void CgiFd::setStatusCode(int const &statusCode)
{
	_statusCode = statusCode;
}

bool CgiFd::getIsHeader()
{
    return _isHeader;
}

void CgiFd::setIsHeader(bool const &stat)
{
	_isHeader = stat;
}

void CgiFd::displayBufferParsed() const
{
	std::cout << "\033[44m==================BUFFER PARSING==================" << std::endl
	<< "==== Header field ===" << std::endl;
	displayStringMap(_header);
	if (_body.size())
		std::cout << std::endl << "==== Body ===" << std::endl << _body << std::endl;
	std::cout << "==============END OF BUFFER PARSING=============\033[0m" << std::endl;
}

void CgiFd::checkError()
{
	stringMapIt	it = _header.find("Status");
	int			statusCode;
	std::string	tmp;
	stringVect	vect;

	if (it == _header.end())
		return;
	tmp = (*it).second;
	vect = splitString(tmp, " ");
	statusCode = convertStringToInt(vect[0]);
	throw ErrorExcept(statusCode, "Error from cgi");
}
