/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileFd.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:36 by barandri          #+#    #+#             */
/*   Updated: 2025/09/18 10:36:51 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileFd.hpp"

int	FileFd::_sockFd[2];

FileFd::FileFd(){}

FileFd::FileFd(int const &fileFd, int const &clientFd, std::string const &filePath) : 
	_fileFd(fileFd),
	_clientFd(clientFd),
	_filePath(filePath),
	_status(OP_OPEN)
{
	_fileType = Mime::getFileType(_filePath);
}

FileFd::FileFd(FileFd const & other)
{
	*this = other;
}

FileFd &FileFd::operator=(FileFd const & other)
{
	if (this != &other)
	{
		_stockFd = other._stockFd;
		_clientFd = other._clientFd;
		_fileFd = other._fileFd;
		_buffer = other._buffer;
		_filePath = other._filePath;
		_fileType = other._fileType;
		_method = other._method;
		_status = other._status;
	}
	return *this;
}

FileFd::~FileFd(){}

int FileFd::getStockFd() const
{
    return _stockFd;
}

int FileFd::getFileFd() const
{
    return _fileFd;
}

int FileFd::getClientFd() const
{
    return _clientFd;
}

std::string FileFd::getBuffer() const
{
    return _buffer;
}

std::string FileFd::getFilePath() const
{
    return _filePath;
}

std::string FileFd::getFileType() const
{
    return _fileType;
}

std::string FileFd::getOriginMethod() const
{
    return _method;
}

int FileFd::getStatus() const
{
    return _status;
}

void FileFd::setStatus(int const &status)
{
	_status = status;
}

void FileFd::setFileType()
{
	_fileType = Mime::getFileType(_filePath);
}

void FileFd::setBuffer(std::string const &content)
{
	_buffer = content;
}

void FileFd::clearBuffer()
{
	_buffer.clear();
}

void FileFd::setupFd()
{
	_stockFd = dup(_fileFd);
	dup2(_sockFd[0], _fileFd);
}

void FileFd::manageFd()
{
	dup2(_stockFd, _fileFd);
}

void FileFd::restoreFd()
{
	dup2(_sockFd[0], _fileFd);
}

void FileFd::initSockFd()
{
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, _sockFd) == -1)
		throw std::runtime_error("Error on socketpair creation");
	write(_sockFd[1], "*", 1);
}

void FileFd::closeSockFd()
{
	close(_sockFd[0]);
	close(_sockFd[1]);
}
