/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:00 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/15 15:21:45 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(Server const & server, const int &_clientFD, bool is_connect) : 
	Server(server),
	_clientFD(_clientFD),
	_connect(is_connect)
{
	this->_clientRequest= "";
	this->_to_send = 0;
	this->_total_send = 0;
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
		this->_fileFD = client._fileFD;
		this->_response = client._response;
		this->_to_send = client._to_send;
		this->_total_send = client._total_send;
	}
	return (*this);
}

Client::~Client(){}

size_t Client::getToSendSize()
{
    return (this->_to_send);
}

size_t Client::getTotalSendSize()
{
    return (this->_total_send);
}

bool Client::is_finishedSendContent()
{
	if (this->_total_send == this->_to_send)
		return (true);
    return (false);
}

void Client::setToSendSize(size_t to_send)
{
	if (!to_send)
		this->_to_send = to_send;
}

void Client::IncrementTotalSendSize(size_t total_send)
{
	this->_total_send += total_send;
}

int Client::get_clientFD() const
{
	return (this->_clientFD);
}

void Client::setCatchedResponse(AExcept & e)
{
	t_errPage	errPage = _config.getErrPage();
	_response.generateCatchedResponse(e, errPage);
}

void Client::setStandardResponse(int const & statusCode)
{
	_response.setType(_fileFD.getFileType());
	_response.setStatusCode(statusCode);
	_response.generateStandardResponse(_fileFD.getBuffer());
}

void Client::ClientRequest()
{
	std::cout << "\033[045m			GET REQUEST " << this->_clientFD << "\033[0m" << std::endl;
	char response[_BUFFER];
	this->_receivedSize = recv(this->get_clientFD(), response, sizeof(response), 0);
	if (_receivedSize == 0)
		this->_connect = false;
	if (_receivedSize == -1)
		throw RecvError();
	this->_clientRequest = response;
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

void Client::setFileFD(const FileFd &fileFd)
{
	_fileFD = fileFd;
}

void Client::linkClientAndFileFd(int const &fd, int const &fdCli, std::string const &filePath)
{
	FileFd _fileFD(fd, fdCli, filePath);

	_fileFD.setFileType();
	_fileFD.setupFd();
}

Response Client::getResponse() const
{
	return (this->_response);
}

void Client::updateResponseStatus(int const &stat)
{
	_response.setRespCategory(stat);
}

FileFd Client::getFileFD() const
{
	return (this->_fileFD);
}

const char *Client::Deconnect::what() const throw()
{
	return ("disconnected!");
}

const char *Client::RecvError::what() const throw()
{
	return ("Error RECV\nget Request from client");
}
