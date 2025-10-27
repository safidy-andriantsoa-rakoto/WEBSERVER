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
	_readStat(OP_OPEN)
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
	_readStat(OP_OPEN)
	{
		_writeStat = OP_START;
		_isError = false;
		_isHeader = false;
		_statusCode = -1;
		_startTime = time(NULL);
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
		_bufferRead = other._bufferRead;
		_readStat = other._readStat;
		_isError = other._isError;
		_statusCode = other._statusCode;
		_isHeader = other._isHeader;
		_header = other._header;
		_body = other._body;
		_bufferWrite = other._bufferWrite;
		_writeStat = other._writeStat;
		_pos = other._pos;
		_startTime = other._startTime;
	}
	return *this;
}

CgiFd::~CgiFd(){}

int CgiFd::getOutputFd() const { return _outputFd;}

void CgiFd::setOutputFd(int const &fd){_outputFd = fd;}

int CgiFd::getErrFd() const { return _errorFd;}

void CgiFd::setErrFd(int const &fd){ _errorFd = fd; }

int CgiFd::getInputFd() const{ return _inputFd;}

void CgiFd::setInputFd(int const &fd){ _inputFd = fd;}

int CgiFd::getClientFd() const { return _clientFd;}

std::string CgiFd::getBuffer() const { return _bufferRead;}

int CgiFd::getReadStat() const { return _readStat;}

void CgiFd::setReadStat(int const &status){_readStat = status;}

bool CgiFd::getIsError() const { return _isError;}

void CgiFd::setIsError(bool const &stat){_isError = stat;}

void CgiFd::setBufferRead(std::string const &content){_bufferRead = content;}

void CgiFd::bufferReadParser()
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
		_body = _bufferRead;
		return;
	}
	startPos = 0;
	pos = 0;
	while (pos != std::string::npos)
	{
		pos = _bufferRead.find(sep, startPos);
		if (pos == startPos)
			break ;
		line = _bufferRead.substr(startPos, pos - startPos);
		headerVect = strictSplit(line, ": ");
		addToMapString(_header, headerVect[0], headerVect[1]);
		startPos = pos + sep.size();
		headerVect.clear();
	}
	_isHeader = true;
	_body = _bufferRead.substr(pos, _bufferRead.size() - pos);
}

void CgiFd::clearBufferRead()
{
	_bufferRead.clear();
	_body.clear();
	_header.clear();
}

std::string CgiFd::getBody() const{return _body;}

std::string CgiFd::getBufferWrite() const{return _bufferWrite;}

void CgiFd::setBufferWrite(std::string const &buff){_bufferWrite = buff;}

int CgiFd::getWriteStat() const{return _writeStat;}

void CgiFd::setWriteStat(int const &stat){_writeStat = stat;}

size_t CgiFd::getPos() const{return _pos;}

void CgiFd::setPos(size_t const &position){_pos = position;}

stringMap CgiFd::getHeader() const{return _header;}

int CgiFd::getStatusCode(){return _statusCode;}

void CgiFd::setStatusCode(int const &statusCode){_statusCode = statusCode;}

bool CgiFd::getIsHeader(){return _isHeader;}

void CgiFd::setIsHeader(bool const &stat){_isHeader = stat;}

void CgiFd::displayBufferReadParsed() const
{
	std::cout << "\033[44m==================BUFFER PARSING==================" << std::endl
	<< "==== Header field ===" << std::endl;
	displayStringMap(_header);
	if (_body.size())
		std::cout << std::endl << "==== Body ===" << std::endl << _body << std::endl;
	std::cout << "==============END OF BUFFER PARSING=============\033[0m" << std::endl;
}

void CgiFd::checkCgiOutput()
{
	stringMapIt	it = _header.find("Status");
	int			statusCode;
	std::string	tmp;
	stringVect	vect;

	if (_writeStat == OP_OPEN && !_header.size())
		throw ErrorExcept(502, "No header from the cgi");
	/*Error 504 if TIMEOUT => CHECK WITH SIGNAL HANDLING*/
	if (it == _header.end())
		return;
	tmp = (*it).second;
	vect = splitString(tmp, " ");
	statusCode = convertStringToInt(vect[0]);
	throw ErrorExcept(statusCode, "Error from cgi after check error");
}

std::string CgiFd::operateOnBufferToWrite()
{
	std::string content;
	size_t		actualPos;

	if (_writeStat == OP_START)
		_pos = 0;
	actualPos = _pos;
	content = divideStringBySize(_bufferWrite, actualPos, _BUFFER);
	actualPos += _BUFFER;
	if (actualPos > _bufferWrite.size())
	{
		_writeStat = OP_CLOSE;
		actualPos = 0;
	}
	_pos = actualPos;
    return content;
}

void CgiFd::checkTimeOut()
{
	time_t	currentTime = time(NULL);

	if (difftime(currentTime, _startTime) > CGI_MAX_TIME)
	{
		// std::cout << "			CGI TIME OUT REACHED!!!!!!!!!!!!!!" << std::endl;
		throw ErrorExcept(504, "CGI Took too long");
	}
}
