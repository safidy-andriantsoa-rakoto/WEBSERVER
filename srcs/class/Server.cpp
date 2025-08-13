/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:55 by barandri          #+#    #+#             */
/*   Updated: 2025/08/13 12:00:12 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//Process request data vs config file and throw exception if needed, redirection
void Server::processRequestWithConfig(Request & request)
{
	//Check primary data
	buildRequestTarget(request);
}

//From config file, build Request Target from the request URI
void Server::buildRequestTarget(Request & request)
{
	vectLocationcIt	it;
	std::string		target;
	vectLocation	loc;

	target = request.getRequestTarget();
	// std::cout << "********TARGET = " << target << "***************" << std::endl;
	if (target == "/")
	{
		request.setRequestTarget(_config.getClassLocation()[0].getRootDirectory() + "/" + "index.html");
		std::cout << "REQUEST TARGET = " << request.getRequestTarget() << std::endl;
		return ;
	}
	loc = _config.getClassLocation();
	it = findLongestMatch(target, loc);
	// std::cout << "LOCATION SELECTED = " << (*it).getLocation() 	<< " => " << (*it).getRootDirectory() << std::endl;
	if ((*it).getLocation() != "/")
		target.replace(target.find((*it).getLocation(), 0), (*it).getLocation().size(), (*it).getRootDirectory());
	else
		target = (*it).getRootDirectory() + target;
	request.setRequestTarget(target);
	std::cout << "REQUEST TARGET = " << request.getRequestTarget() << std::endl;
}

Server::Server() {}

Server::Server(Server const & other)
{
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

Server::~Server()
{
	if (_fdClient != -1)
		close(_fdClient);
	if (_fdSocket != -1)
		close(_fdSocket);
}

MyConf Server::getConfig() const
{
	return _config;
}

int Server::get_fdClient() const
{
	return (this->_fdClient);
}

int Server::get_fdSocket() const
{
    return _fdSocket;
}

SockAddrIn Server::get_confSockAddrIn() const
{
    return (this->_sockaddrIn);
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
	int fd = accept(_fdSocket, (sockaddr *)&(this->_sockaddrIn), (socklen_t *)&(this->_size_sockaddrin));
	if (this->_fdClient == -1)
	{
		std::cout << "Error\naccept a connection on a socket" << std::endl;
		return (-2);
	}
	return (fd);
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

void Server:: processRequest(std::string const & clientRequest, int const & fd_cli)
{
	Request		request;
	Response	response;
    std::string responseString;

	if (!clientRequest.length())
		return ;
	try
	{
		request.parseRequest(clientRequest);
		processRequestWithConfig(request);
		if (request.getMethod() == "GET")
			response = get_request(request); 
		else if (request.getMethod() == "POST")
			std::cout << request.getMethod() << std::endl; // Calling post_request
		else if (request.getMethod() == "DELETE")
			std::cout << request.getMethod() << std::endl; // Calling delete_request
	}
	catch(ErrorExcept const & e)
	{
		std::cerr << "Process Request error \n" << e._error << std::endl;
		response.responseErrorGenerator(e._status, ERR_PATH);
	}
	responseString = response.getFullResponse();
	send_value(fd_cli, responseString);
}

Response Server::get_request(Request & request)
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
		throw ErrorExcept(404, "File unreachable");
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
	Response response(200, type);
	response.responseGenerator(request.getRequestTarget());
	return response;
}

Response Server::post_request(Request &request)
{
	(void) request;
    return Response();
}

Response Server::delete_request(Request &request)
{
	(void) request;
    return Response();
}

void Server::simpleInit(int const & port, std::string const & locationPath)
{
	MyConf conf(port);

	conf.parseConfig(locationPath);
	_config = conf;
}

vectLocationIt	Server::findLongestMatch(std::string const & text, vectLocation & location)
{
	std::string		compare;
	vectLocationIt	itFinal;
	stringVect		vect1;
	stringVect		vect2;
	int				nbMatch = 0;
	int				nbMatchMax = 0;

	vect1 = splitString(text, "/");
	itFinal = location.begin();
	for(vectLocationIt it = location.begin(); it != location.end(); it++)
	{
		compare = (*it).getLocation();
		if (text == compare)
		return it;
		vect2 = splitString(compare, "/");
		nbMatch = 0;
		for(size_t i = 0; i < vect1.size() && i < vect2.size(); i++)
		{
			if (vect1[i] != vect2[i])
				break;
			nbMatch++;
		}
		if (nbMatch > nbMatchMax)
		{
			nbMatchMax = nbMatch;
			itFinal = it;
		}
	}
	return itFinal;
}