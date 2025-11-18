/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:55 by barandri          #+#    #+#             */
/*   Updated: 2025/11/18 09:36:50 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() : _fdClient(-1), _fdSocket(-1){}

Server::Server(MyConf conf): _fdClient(-1), _fdSocket(-1), _config(conf){}

Server::Server(Server const & other)
{
	_fdClient = -1;
	_fdSocket = -1;
	*this = other;
}

Server &Server::operator=(Server const & other) 
{
	if (this != &other)
	{
		_config = other._config;
		_fdSocket = other._fdSocket;
		_fdClient = other._fdClient;
		_size_sockaddrin = other._size_sockaddrin;
		_sockaddrIn = other._sockaddrIn;
		_addrinfo = other._addrinfo;
	}
	return *this;
}

Server::~Server(){}

MyConf Server::getConfig() const{return _config;}

int Server::get_fdClient() const{return (this->_fdClient);}

int Server::get_fdSocket() const{return _fdSocket;}

SockAddrIn & Server::get_confSockAddrIn(){return (_sockaddrIn);}

MyConf &Server::config_ref(){return (_config);}

int Server::create_socket()
{
	int opt = 1;

	this->_fdSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (setsockopt(this->_fdSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
		return 1;
	}
	if (this->_fdSocket == -1)
	{
		std::cerr << "Error\ncreate an endpoint for communication" << std::endl;
		return 0; 
	}
	return 1;
}

void Server::configure_sockaddrin(int port)
{
	this->_sockaddrIn.sin_family = AF_INET;
	this->_sockaddrIn.sin_port = htons(port);
	this->_sockaddrIn.sin_addr.s_addr = INADDR_ANY;
	this->_size_sockaddrin = sizeof(_sockaddrIn);
}

int Server::link_socket_adress()
{
	int     status;

	status = bind(this->_fdSocket, (sockaddr *)&(this->_sockaddrIn), this->_size_sockaddrin);
	if (status == -1)
	{
		perror(strerror(errno));
		std::cout << "Error\nbind a name to a socket" << std::endl;
		return (0);
	}
	return (1);
}

int Server::resolve_host(std::string const &host, int const &port)
{
	memset(&_addrinfo, 0, sizeof(_addrinfo));
	_addrinfo.ai_family = AF_INET;
	_addrinfo.ai_socktype = SOCK_STREAM;

	std::stringstream port_stream;
	port_stream << port;
	std::string port_str = port_stream.str();
	int stat = getaddrinfo(host.c_str(), port_str.c_str(), &_addrinfo, &_resp);
	if (stat != 0)
	{
		std::cerr << "getaddrinfo: " << gai_strerror(errno) << " or Invalid HostName." << std::endl;
		return (0);
	}
	if (_resp && _resp->ai_family == AF_INET)
	{
		memset(&_sockaddrIn, 0, sizeof(_sockaddrIn));
		_sockaddrIn.sin_addr = ((SockAddrIn *)_resp->ai_addr)->sin_addr;
		_sockaddrIn.sin_family = AF_INET;
		_sockaddrIn.sin_port = htons(port);
		_size_sockaddrin = sizeof(_sockaddrIn);
		freeaddrinfo(_resp);
		return (1);
	}
    return (0);
}

int Server::wait_connexion_from_client()
{
	this->_fdClient = listen(this->_fdSocket, SOMAXCONN);
	std::cout << "Wait..." << std::endl; /* [Test only] */
	if (this->_fdClient == -1)
	{
		std::cerr << "Error\nlisten for connections on a socket" << std::endl;
		return (0);    
	}
	return (1);
}

int Server::accept_client_connexion()
{
	int fd = accept(_fdSocket, (sockaddr *)&(this->_sockaddrIn), &(this->_size_sockaddrin));
	if (fd == -1)
	{
		perror("Accept");
		throw std::runtime_error("Fd error");
	}
	return (fd);
}

std::string Server::get_client_request()
{
	char response[_BUFFER];
	int _respStatus = recv(this->_fdClient, response, sizeof(response), 0);
	if (_respStatus == -1)
	{
		std::cout << "Error\nreceive a message from a socket" << std::endl;
		return ("");
	}
	return (response);
}

void Server::clearAllFd()
{
	if (_fdClient != -1)
		close(_fdClient);
	if (_fdSocket != -1)
		close(_fdSocket);
}


