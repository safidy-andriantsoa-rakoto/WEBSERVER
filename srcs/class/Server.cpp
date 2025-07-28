/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:55 by barandri          #+#    #+#             */
/*   Updated: 2025/07/28 11:22:01 by barandri         ###   ########.fr       */
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
        _root = other._root;
        _location = other._location;
        _networkSocket = other._networkSocket;
        _requestList = other._requestList;
    }
    return *this;
}

Server::~Server()
{
    if (this->_fdClient != -1)
        close(this->_fdClient);
    if (this->_fdSocket != -1)
        close(this->_fdSocket);
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

std::string Server::getRoot() const
{
    return _root;
}

std::string Server::getLocation() const
{
    return _location;
}

intVect Server::getNetworkSocket() const
{
    return _networkSocket;
}

RequestList Server::getRequestList() const
{
    return _requestList;
}

void Server::addRequest(Request request)
{
    _requestList.push_back(request);
}

int Server::get_fdClient()
{
    return (this->_fdClient);
}

int Server::creat_socket()
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
    this->_fdClient = listen(this->_fdSocket, _NB_CLIENT);
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
    this->_fdClient = accept(_fdSocket, (sockaddr *)&(this->_sockaddrIn), (socklen_t *)&(this->_size_sockaddrin));
    if (this->_fdClient == -1)
    {
        std::cout << "Error\naccept a connection on a socket" << std::endl;
        return (0);
    }
    return (1);
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

void Server::get_request(Request &request, int &_acceptStatus)
{
	std::string ext_file;
	std::string	line;
	std::string f_line;
	std::string tmp_ext;
	std::string content;
	std::string response;

	tmp_ext = request.getRequestTarget();
	line = tmp_ext;
	int pe = tmp_ext.find_last_of(".");
	std::cout << "Find . pos:" << pe << std::endl;
	ext_file = tmp_ext.erase(0, pe + 1);
	std::cout << "extension->" << ext_file << std::endl;
	std::string path = "test/";
	line.erase(0,1);
	if (ext_file == "/")
	{
		content = readFileToString(path + "index.html"); //Nampiana path
		std::cout << "CONTENT = " << content << std::endl;
        response = response_generator("HTTP/1.1 200 OK\r\n", "text/html", content);// line request + content index.html
		std::cout << "\n" << response << std::endl;
		send_value(_acceptStatus, response);
	}
	else if (ext_file == "css" || ext_file == "html" || ext_file == "plain")
	{
		std::cout << "aty ee" << std::endl;
		path += line;
		content = readFileToString(path);
		std::string type = "text/";
		type += ext_file;
		response = response_generator("HTTP/1.1 200 OK\r\n", type, content);
		send_value(_acceptStatus, response);
	}
	else if (ext_file == "js")
	{
		path += line;
		content = readFileToString(path);
		response = response_generator("HTTP/1.1 200 OK\r\n", "text/javascript", content);
		send_value(_acceptStatus, response);
	}
	else if (ext_file == "jpg" || ext_file == "png" || ext_file == "jpeg" || ext_file == "ico")
	{
		path += line;
		content = readFileToString(path);
		std::string ext = "image/";
		ext += ext_file;
		response = response_generator("HTTP/1.1 200 OK\r\n", ext, content);
		send_value(_acceptStatus, response);
	}
	else if (ext_file == "woff" || ext_file == "pdf")
	{
		path += line;
		content = readFileToString(path);
		std::string ext = "application/";
		ext += ext_file;
		response = response_generator("HTTP/1.1 200 OK\r\n", ext, content);
		send_value(_acceptStatus, response);
	}
	else if (ext_file == "mp4")
	{
		path += line;
		content = readFileToString(path);
		std::string ext = "video/";
		ext += ext_file;
		response = response_generator("HTTP/1.1 200 OK\r\n", ext, content);
		send_value(_acceptStatus, response);
	}
}