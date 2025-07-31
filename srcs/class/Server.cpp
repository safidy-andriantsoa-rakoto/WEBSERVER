/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:55 by barandri          #+#    #+#             */
/*   Updated: 2025/07/31 15:31:29 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(){}

Server::Server(Server & other)
{
	*this = other;
}

Server &Server::operator=(Server &other)
{
	if (this != &other)
	{
		_port = other._port;
		_host = other._host;
		_serverName = other._serverName;
		_location = other._location;
		_fdSocket = other._fdSocket;
		_fdClient = other._fdClient;
		_size_sockaddrin = other._size_sockaddrin;
		_sockaddrIn = other._sockaddrIn;
	}
	return *this;
}

Server::~Server()
{
	if (_fdClient != -1)
		close(_fdClient);
	if (_fdSocket != -1)
		close(_fdSocket);
}

std::string Server::getPort() const
{
	return _port;
}

std::string Server::getHost() const
{
	return _host;
}

std::string Server::getServerName() const
{
	return _serverName;
}

stringMap Server::getLocation() const
{
	return _location;
}

std::string Server::getGeneralRoot() const
{
    return _generalRoot;
}

int Server::get_fdClient()
{
	return (this->_fdClient);
}

//Afaka atao throw
int Server::create_socket()
{
	this->_fdSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fdSocket == -1)
	{
		std::cout << "Error\ncreate an endpoint for communication" << std::endl;
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

//Afaka atao throw
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

//Afaka atao throw
int Server::wait_connexion_from_client()
{
	this->_fdClient = listen(this->_fdSocket, _NB_CLIENT);
	std::cout << "Wait..." << std::endl;
	if (this->_fdClient == -1)
	{
		std::cout << "Error\nlisten for connections on a socket" << std::endl;
		return (0);    
	}
	return (1);
}

//Afaka atao throw
int Server::accept_client_connexion()
{
	this->_fdClient = accept(_fdSocket, (sockaddr *)&(this->_sockaddrIn), (socklen_t *)&(this->_size_sockaddrin));
	if (this->_fdClient == -1)
	{
		std::cout << "Error\naccept a connection on a socket" << std::endl;
		return (0);
	}
	return (1);
}

//Afaka atao throw
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

void Server::get_request(Request & request, Response & response)
{
	std::string		line;
	std::string 	tmp_ext;
	std::string 	ext_file;
	std::string 	type;
	std::fstream	file;

	tmp_ext = request.getRequestTarget();
	//Use of config file parameter to search for the file!!!
	//On error : need to specify the statusCode (no need to throw?)
	file.open(tmp_ext.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error on get request");
	//Calling a generic function to get type based on file extension
	ext_file = getFileExtension(tmp_ext);
	std::cout << "extension->" << ext_file << std::endl;
	if (ext_file == "/")
		type = "text/html";
	else if (ext_file == "css" || ext_file == "html" || ext_file == "plain")
		type = "text/" + ext_file;
	else if (ext_file == "js")
		type = "text/javascript";
	else if (ext_file == "jpg" || ext_file == "png" || ext_file == "jpeg" || ext_file == "ico" || ext_file == "svg")
	{
		type = "image/";
		if (ext_file == "svg")
			type += "svg+xml";  
		else
			type += ext_file;
	}
	else if (ext_file == "woff" || ext_file == "pdf")
		type = "application/" + ext_file;
	else if (ext_file == "mp4")
		type = "video/" + ext_file;
	response.setStatusCode(200);
	response.setType(type);
}

void Server::simpleInit()
{
	_port = convertIntToString(2003);
	_host = "localhost";
	_serverName = "_";
	// addToMapString(_location, "/", "/home/barandri/site/agency");
	// addToMapString(_location, "/images", "/home/barandri/site/agency/assets/images");
	// addToMapString(_location, "/css", "/home/barandri/site/agency/assets/css");
	// addToMapString(_location, "/js", "/home/barandri/site/agency/assets/js");
	// addToMapString(_location, "/webfonts", "/home/barandri/site/agency/assets/webfonts");

	addToMapString(_location, "/", "/home/barandri/site/dim");
	addToMapString(_location, "/images", "/home/barandri/site/dim/images");
	addToMapString(_location, "/css", "/home/barandri/site/dim/assets/css");
	addToMapString(_location, "/js", "/home/barandri/site/dim/assets/js");
	addToMapString(_location, "/webfonts", "/home/barandri/site/dim/assets/webfonts");
	addToMapString(_location, "/sass", "/home/barandri/site/dim/assets/sass");
}
