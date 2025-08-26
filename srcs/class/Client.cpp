/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:00 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/26 11:11:35 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(const int &_parentFD, const int &_clientFD,const SockAddrIn& _configSockAddrIn, bool is_socketDF) : _parentFD(_parentFD),
_clientFD(_clientFD), _configSockAddrIn(_configSockAddrIn), _server_socket(is_socketDF)
{
    _client_stat = -1000;
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
    this->_client_stat = recv(this->get_clientFD(), response, sizeof(response), 0);
    if (this->_client_stat == 0)
    {
        // std::cout << "      \033[44mLE CLIENT EST DECONNECTE\033[0m" << std::endl;
        // return("");
        // close(this->_clientFD);
        throw Deconnect();
    }
    if (this->_client_stat == -1)
    {
        // std::cout << "Error\nreceive a message from a socket" << std::endl;
        // perror("Error from client");
        throw RecvError();
        // return ("");
    }
    return (response);
}

Client::Client(const Client &client)
{
    *this = client;
}

Client::~Client()  {}

const char *Client::Deconnect::what() const throw()
{
    return ("\033[43mLE CLIENT EST ACTUELLEMENT DECONNECTE.\033[0m \n");
}


const char *Client::RecvError::what() const throw()
{
    return ("\033[43mERROR_RECV\033[0m \n");
}