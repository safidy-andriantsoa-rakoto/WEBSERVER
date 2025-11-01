/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:55 by barandri          #+#    #+#             */
/*   Updated: 2025/10/31 08:27:49 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() : _fdClient(-1), _fdSocket(-1){}

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
	}
	return *this;
}

Server::~Server(){}

MyConf Server::getConfig() const{return _config;}

int Server::get_fdClient() const{return (this->_fdClient);}

int Server::get_fdSocket() const{return _fdSocket;}

SockAddrIn & Server::get_confSockAddrIn(){return (_sockaddrIn);}

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
		return (0); 
	}
	return (1);
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
		std::cout << "Error\nbind a name to a socket" << std::endl;
		return (0);
	}
	return (1);
}

int Server::wait_connexion_from_client()
{
	this->_fdClient = listen(this->_fdSocket, SOMAXCONN);
	std::cout << "Wait..." << std::endl;
	if (this->_fdClient == -1)
	{
		std::cout << "Error\nlisten for connections on a socket" << std::endl;
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

void Server::simpleInit(std::string const & servPath, std::string const & locationPath)
{
	MyConf conf;

	conf.myParseConfig(servPath, locationPath);
	_config = conf;
}

void Server::getParsAllServer(std::vector<Server> &allServer, stringVect file)
{
	size_t i(0);
	while(i < allServer.size())
	{
		allServer[i]._config.parseConfig(file);
		allServer[i]._config.convertSizeMax(); /* [?] : Need to check pc size available */
		allServer[i]._config.displayConfig();
		if(allServer[i]._config.getStatus() == true)
			i++;
		else
			return ;
	}
}

std::vector<int> Server::stockPort(std::vector<Server> &allServer)
{
	size_t i(0);
	intVect tabPort;
	intVect port;
	while (i < allServer.size())
	{
		port = allServer[i]._config.getServPort();
		for (size_t i = 0; i < port.size(); i++)
		{
			tabPort.push_back(port[i]);
		}
		i++;
	}

	return (tabPort);
}

bool Server::checkDuplicateValue(std::vector<Server> allserver)
{
	intVect port;
	size_t i(0), j(0);

	port = Server::stockPort(allserver);
	std::cout << "Port list ";
	while (i < port.size())
	{
		j = i + 1;
		while (j < port.size())
		{
			// std::cout << port[i] << " " << port[j] << std::endl;
			if (port[i] == port[j])
			{
				std::cerr << "Error : find duplicate port " << port[i] << std::endl;
				return (false);
			}
			j++;
		}
		i++;
	}

	for (size_t i = 0; i < port.size(); i++)
	{
		std::cout << port[i] << " ";
	}

	std::cout << std::endl;
	return (true);
}
