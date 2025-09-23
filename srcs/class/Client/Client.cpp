/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:00 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/23 08:54:21 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(Server const & server, const int &_clientFD, bool is_connect) : 
	Server(server),
	_clientFD(_clientFD),
	_connect(is_connect),
	_fileFD(NULL)
{
	this->_clientRequest= "";
	if (_clientFD < 0)
	{
		std::cout << "Error\nInvalid input FD" << std::endl; 
		return ;
	}
}

Client::Client(const Client &client) : Server(client)
{
	*this = client;
}

Client &Client::operator=(const Client &client)
{
	if (this != &client)
	{
		this->Server::operator=(client);
		this->_clientFD = client._clientFD;
		this->_connect = client._connect;
		this->_clientRequest = client._clientRequest;
		this->_client_stat = client._client_stat;
		this->_response = client._response;
		this->_fileFD = client._fileFD;
	}
	return (*this);
}

Client::~Client(){}

int Client::get_clientFD() const
{
	return (this->_clientFD);
}

void Client::setCatchedResponse(AExcept & e, int & fd, std::string & filePath)
{
	t_errPage	errPage = _config.getErrPage();
	_response.generateCatchedResponse(e, errPage, fd, filePath);
}

void Client::setStandardResponse(int const & statusCode)
{
	_response.setType(_fileFD->getFileType());
	_response.setStatusCode(statusCode);
	_response.generateStandardResponse(_fileFD->getBuffer());
}

void Client::setChunkedResponse(int const &statusCode)
{	
	_response.setStatusCode(statusCode);
	if (_fileFD)
	{
		_response.setType(_fileFD->getFileType());
		_response.generateChunkedResponse(_fileFD->getBuffer(), _fileFD->getStatus());
	}
}

void Client::ClientRequest()
{
	char response[_BUFFER_RECV];
	int _respStatus = recv(this->get_clientFD(), response, sizeof(response), 0);
	if (_respStatus == 0)
		this->_connect = false;
	if (_respStatus == -1)
		throw RecvError();
	this->_clientRequest = std::string(response, _respStatus);
}

std::string Client::getClientRequest() const
{
	if (this->_clientRequest.size())
		return (this->_clientRequest);
	return ("");
}

bool Client::getConnexionStat()
{
	return (this->_connect);
}

void Client::setFileFD(FileFd *fileFd)
{
	if (_fileFD)
		delete _fileFD;
	_fileFD = fileFd;
}

void Client::updateFileStatus(int const &status)
{
	_fileFD->setStatus(status);
}

void Client::increaseFileSent(size_t const &add)
{
	_fileFD->increaseFileSent(add);
}

Response & Client::getResponse()
{
	return (this->_response);
}

std::string Client::getFullResponse()
{
	return _response.getFullResponse();
}

std::string Client::getBodyResponse()
{
    return _response.getBody();
}

int Client::getCategoryResponse()
{
    return _response.getRespCategory();
}

int Client::getStatusCodeResponse()
{
    return _response.getStatusCode();
}

void Client::clearResponse()
{
	_response.clearResponse();
}

void Client::updateResponseCategory(int const &stat)
{
	_response.setCategory(stat);
}

FileFd *Client::getFileFD()
{
	return (_fileFD);
}

int Client::getFileStatus()
{
	return _fileFD->getStatus();
}

const char *Client::Deconnect::what() const throw()
{
	return ("disconnected!");
}

const char *Client::RecvError::what() const throw()
{
	return ("Error RECV\nget Request from client");
}
