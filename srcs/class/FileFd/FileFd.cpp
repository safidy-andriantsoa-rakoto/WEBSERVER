/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileFd.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:36 by barandri          #+#    #+#             */
/*   Updated: 2025/10/13 10:47:48 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileFd.hpp"

int	FileFd::_sockFd[2] = {-1, -1};

FileFd::FileFd(){}

FileFd::FileFd(int const &fileFd, int const &clientFd, std::string const &filePath) : 
	_fileFd(fileFd),
	_clientFd(clientFd),
	_fileTotalSize(0),
	_fileSent(0),
	_filePath(filePath),
	_status(OP_OPEN),
	_EndFilepost(false),
	_fdMPFDT(-1)
{
	std::cout << "		PATH-->" << _filePath << std::endl;
	_fileSent = 0;
	_stockFd = -1;
	_fileType = "cgi";
	_fileTotalSize = 0;
	_fileType = Mime::getFileType(_filePath);
	_fileTotalSize = getFileSize(_filePath);
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
		_fileTotalSize = other._fileTotalSize;
		_fileSent = other._fileSent;
		_EndFilepost = other._EndFilepost;
		_fdMPFDT = other._fdMPFDT;
	}
	return *this;
}

FileFd::~FileFd(){}

int FileFd::getStockFd() const
{
    return _stockFd;
}

int FileFd::getFd() const
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

void FileFd::setFileTotalSize(size_t size)
{
	if ((int)size > -1)
		this->_fileTotalSize += size;
}

void FileFd::setFileSentSize(size_t size)
{
	if ((int)size > -1)
		this->_fileSent = size;
}

size_t FileFd::getFileTotalSize() const
{
    return _fileTotalSize;
}

size_t FileFd::getFileSentSize() const
{
    return _fileSent;
}

void FileFd::increaseFileSent(size_t const &add)
{
	_fileSent += add;
}

bool FileFd::get_endFilePost()
{
    return (_EndFilepost);
}

void FileFd::set_endFilePost(bool stat)
{
	this->_EndFilepost = stat;
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
	if (_stockFd == -1)
		throw std::runtime_error("setupFd() : dup error");
	if (dup2(_sockFd[0], _fileFd) == -1)
		throw std::runtime_error("setupFd() : dup2 error");
}

void FileFd::manageFd()
{
	if (dup2(_stockFd, _fileFd) == -1)
		throw std::runtime_error("manageFd() : dup2 error");
}

void FileFd::restoreFd()
{
	if (dup2(_sockFd[0], _fileFd) == -1)
		throw std::runtime_error("restoreFd() : dup2 error");
}

void FileFd::initSockFd()
{
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, _sockFd) == -1)
		throw std::runtime_error("Error on socketpair creation");
	write(_sockFd[1], "*", 1);
}

void FileFd::closeSockFd()
{
	if (_sockFd[0] != -1)
		close(_sockFd[0]);
	if (_sockFd[1] != -1)
		close(_sockFd[1]);
}

void FileFd::set_post_body(std::string &body)
{
	if (body.size())
		_buffer.insert(_buffer.size(), body);
}

void FileFd::set_boundary(std::string &boundary)
{
	this->_boundary = boundary;
}

std::string &FileFd::get_boundary()
{
    return (this->_boundary);
}

std::string FileFd::get_range_body(int start, int end)
{
	if (_buffer.size() && start > -1 && end > -1 && end > start)
    	return (_buffer.substr(start, end));
	return ("");
}

void FileFd::delete_range_body(int start, int end)
{
	if (_buffer.size() && start > -1 && end > -1 && end > start)
	{
		if ((size_t)(end) > _buffer.size())
			_buffer.erase(_buffer.begin() + start, _buffer.end());
		else
			_buffer.erase(_buffer.begin() + start, _buffer.begin() + end);
	}
}
