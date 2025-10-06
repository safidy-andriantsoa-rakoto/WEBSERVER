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
	_cgiFd(CgiFd),
	_clientFd(clientFd),
	_status(OP_OPEN)
	{
		_isError = false;
	}

CgiFd::CgiFd(int const &cgiFd, int const & cgiErrFd, int const &clientFd) : 
	_cgiFd(cgiFd),
	_cgiErrFd(cgiErrFd),
	_clientFd(clientFd),
	_status(OP_OPEN)
	{
		_isError = false;
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
		_cgiFd = other._cgiFd;
		_cgiErrFd = other._cgiErrFd;
		_buffer = other._buffer;
		_status = other._status;
		_isError = other._isError;
	}
	return *this;
}

CgiFd::~CgiFd(){}

int CgiFd::getFd() const
{
    return _cgiFd;
}

int CgiFd::getErrFd() const
{
    return _cgiErrFd;
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

void CgiFd::clearBuffer()
{
	_buffer.clear();
}

