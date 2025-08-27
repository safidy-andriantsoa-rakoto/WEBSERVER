/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:00 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/27 14:15:26 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(const int &_parentFD, const int &_clientFD,const SockAddrIn& _configSockAddrIn, bool is_socketDF) : _parentFD(_parentFD),
_clientFD(_clientFD), _configSockAddrIn(_configSockAddrIn), _server_socket(is_socketDF)
{
    if (_clientFD < 0 || _parentFD < 0)
    {
        std::cout << "Error\nInvalid input FD" << std::endl; 
        return ;
    }
}

int Client::get_clientFD() const
{
    return (this->_clientFD);
}

int Client::get_parent() const
{
    return (this->_parentFD);
}

Client &Client::operator=(const Client &client)
{
    this->_clientFD = client._clientFD;
    this->_parentFD = client._parentFD;
    this->_configSockAddrIn = client._configSockAddrIn;
    this->_server_socket = client._server_socket;
    return (*this);
}

std::string Client::ClientRequest()
{
    char response[_BUFFER];
    int _respStatus = recv(this->get_clientFD(), response, sizeof(response), 0);
    if (_respStatus == -1)
    {
        std::cout << "Error\nreceive a message from a socket" << std::endl;
        return ("");
    }
    return (response);
}

Client::Client(const Client &client)
{
    *this = client;
}

Client::~Client() {}
