/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:00 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/09 13:30:20 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(const int &_parentFD, const int &_clientFD,const SockAddrIn& _configSockAddrIn, bool is_connect) : _parentFD(_parentFD),
_clientFD(_clientFD), _configSockAddrIn(_configSockAddrIn), _connect(is_connect)
{
    this->_clientRequest= "";
    if (_clientFD < 0 || _parentFD < 0)
    {
        std::cout << "Error\nInvalid input FD" << std::endl; 
        return ;
    }
}

Client::Client(Server const &server, int const &fdCli)
{
    _parentFD = server.get_fdSocket();
    _clientFD = fdCli;
    _configSockAddrIn = server.get_confSockAddrIn();
    _connect = true;
}

int Client::get_clientFD() const
{
    return (this->_clientFD);
}

int Client::get_fdparent() const
{
    return (this->_parentFD);
}

Client &Client::operator=(const Client &client)
{
    this->_clientFD = client._clientFD;
    this->_parentFD = client._parentFD;
    this->_configSockAddrIn = client._configSockAddrIn;
    this->_connect = client._connect;
    this->_fileFD = client._fileFD;
    this->_clientRequest = client.getClientRequest();
    return (*this);
}

void Client::ClientRequest()
{
    char response[_BUFFER];
    int _respStatus = recv(this->get_clientFD(), response, sizeof(response), 0);
    if (_respStatus == 0)
        this->_connect = false;
    if (_respStatus == -1)
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
    this->_fileFD = fileFd;
}

FileFd Client::getFileFD()
{
    return (this->_fileFD);
}

Client::Client(const Client &client)
{
    *this = client;
}

Client::~Client() {}

const char *Client::Deconnect::what() const throw()
{
    return ("disconnected!");
}

const char *Client::RecvError::what() const throw()
{
    return ("Error RECV\nget Request from client");
}
