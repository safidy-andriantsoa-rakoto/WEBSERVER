/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileFd.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:36 by barandri          #+#    #+#             */
/*   Updated: 2025/09/09 10:26:37 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileFd.hpp"

FileFd::FileFd(){}

FileFd::FileFd(int const &fileFd, int const &clientFd, std::string const &filePath) : _fileFd(fileFd), _clientFd(clientFd), _filePath(filePath) {}

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

int *FileFd::getSockFd()
{
    return _sockFd;
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

void FileFd::setFileType()
{
	_fileType = Mime::getFileType(_filePath);
}

void FileFd::setBuffer(std::string const &content)
{
	_buffer = content;
}

void FileFd::setupFd()
{
	int	returnVal;

	returnVal = socketpair(AF_UNIX, SOCK_STREAM, 0, _sockFd);
	if (returnVal == -1)
		throw std::runtime_error("Error on socketpair creation");
	write(_sockFd[1], "*", 1);
	_stockFd = dup(_fileFd);
	dup2(_sockFd[0], _fileFd);
}

void FileFd::manageFd()
{
	dup2(_stockFd, _fileFd);
}

void FileFd::closeAllOwnedFileFd()
{
	close(_stockFd);
	close(_fileFd);
	close(_sockFd[0]);
	close(_sockFd[1]);
}
