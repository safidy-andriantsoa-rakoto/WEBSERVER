/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:55 by barandri          #+#    #+#             */
/*   Updated: 2025/09/18 21:37:03 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//Process request data vs config file and throw exception if needed, redirection
void Server::processRequestWithConfig(Request & request)
{
	checkContentLength(request);
	buildRequestTarget(request);
}

void Server::checkContentLength(Request &request)
{
	float		maxSize;
	float		requestSize;
	stringMapIt	itFind;
	stringMap	header = request.getHeader();

	itFind = header.find("Content-Length");
	if (itFind == header.end())
		return ;
	maxSize = _config.getMaxSize();
	requestSize = convertStringToFLoat((*itFind).second);
	if (requestSize > maxSize)
		throw ErrorExcept(413, "Size limit not respected");
}

//From config file, build Request Target from the request URI
//Index? Default file if directory
void Server::buildRequestTarget(Request & request)
{
	vectLocationIt	it;
	std::string		target;
	vectLocation	loc;
	std::string		autoIndex;

	target = request.getRequestTarget();
	std::cout << "\033[96m********TARGET = " << target << "***************\033[0m" << std::endl;
	if (target == "/")
	{
		processLocationOption(_config.getClassLocation()[0], request);
		request.setRequestTarget(_config.getClassLocation()[0].getRootDirectory() + "/" + "index.html");
		std::cout << "\033[96mREQUEST TARGET = " << request.getRequestTarget() << "\033[0m" << std::endl;
		return ;
	}
	loc = _config.getClassLocation();
	it = findLongestMatch(target, loc);
	processLocationOption(*it, request);
	std::cout << "\033[96mLOCATION SELECTED = " << (*it).getLocation() << " => " << (*it).getRootDirectory() << "\033[0m" << std::endl;
	if ((*it).getLocation() != "/")
		target.replace(target.find((*it).getLocation(), 0), (*it).getLocation().size(), (*it).getRootDirectory());
	else
		target = (*it).getRootDirectory() + target;
	autoIndex = (*it).getAutoindex();
	if (autoIndex == "true")
		request.setAutoIdx(true);
	request.setRequestTarget(target);
	std::cout << "\033[96mREQUEST TARGET = " << request.getRequestTarget() << "\033[0m" << std::endl;
}

void Server::processLocationOption(MyLoc const & location, Request & request)
{
	stringVect	locationMethod;

	locationMethod = location.getAvailableMethod();
	processRedirection(location, request);
	checkMethodInLocation(locationMethod, request.getMethod());
}

void Server::checkMethodInLocation(stringVect & locationMethod, std::string const & method)
{
	stringVectIt	it;

	it = std::find(locationMethod.begin(), locationMethod.end(), method);
	if (it == locationMethod.end())
	{
		if (method != "GET" && method != "POST" && method != "DELETE")
			throw ErrorExcept(501, "Method not implemented");
		else
			throw ErrorExcept(405, "Method not allowed");
	}
}

// Review redirection function
void Server::processRedirection(MyLoc const & location, Request & request)
{
	std::string	oldUri;
	std::string	newUri;
	stringVect	redirect;
	int			statusRedirect;

	redirect = location.getRedirect();
	if (!redirect.size())
	{
		// std::cout << "\033[31m<<<<<<NOOOO REDIRECTION>>>>>>\033[0m" << std::endl;
		return ;
	}
	std::cout << "\033[32m<<<<<<<<<REDIRECTION HEREEE>>>>>>>>>\033[0m" << std::endl;
	redirect = location.getRedirect();
	statusRedirect = convertStringToInt(redirect[0]);
	newUri = redirect[1];
	if (newUri.find("$uri") != std::string::npos)
	{
		oldUri = request.getRequestTarget();
		oldUri.erase(oldUri.find(location.getLocation()), location.getLocation().size() + 1);
		replaceString(newUri, oldUri, "$uri");
	}
	// std::cout << "Old = " << oldUri << " => " << newUri << std::endl;
	throw RedirExcept(statusRedirect, "Redirection", newUri);
}

Server::Server(){}

Server::Server(Server const & other) :
	_config(other._config),
	_fdSocket(other._fdSocket),
	_fdClient(other._fdClient),
	_size_sockaddrin(other._size_sockaddrin),
	_sockaddrIn(other._sockaddrIn)
{
	// *this = other;
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
	int opt = 1;

	this->_fdSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (setsockopt(this->_fdSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
		return 1;
	}
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
		std::cout << "Error\naccept a connection on a socket" << std::endl;
		return (-1);
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

int Server:: processRequest(std::string const & clientRequest, uint32_t & opt, std::string & filePath)
{
	int			fileFd;
	Request		request;
	Response	response;

	fileFd = -1;
	request.parseRequest(clientRequest);
	processRequestWithConfig(request);
	if (request.getMethod() == "GET")
	{
		fileFd = get_request(request);
		filePath = request.getRequestTarget();
		opt = EPOLLIN;
	}
	else if (request.getMethod() == "POST")
	{
		std::cout << request.getMethod() << std::endl; // Calling post_request
		opt = EPOLLOUT;
	}
	else if (request.getMethod() == "DELETE")
		std::cout << request.getMethod() << std::endl; // Calling delete_request
	return fileFd;
}

int Server::get_request(Request & request)
{
	int			fd;
	std::string	filePath;
	DIR*		dir;

	filePath = request.getRequestTarget();
	dir = opendir(filePath.c_str());
	if (!dir && errno == EACCES)
		throw ErrorExcept(403, "Directory forbidden");
	if (dir && request.getAutoIdx() == true)
		throw DirExcept(300, dir, filePath, "Listing directory");
	if (dir && request.getAutoIdx() == false)
	{
		filePath += "/index.html";
		closedir(dir);
	}		
	fd = open(filePath.c_str(), O_RDONLY);
	std::cout << "\033[046mFD=" << fd << "\033[0m" << std::endl;
	if (fd == -1)
	{
		perror("Open file");
		if (errno == EACCES)
			throw ErrorExcept(403, "No permission for file");
		throw ErrorExcept(404, "File can't be opened");
	}
	return fd;
}

int Server::post_request(Request &request)
{
	(void) request;
	return 0;
}

void Server::delete_request(Request &request)
{
	(void) request;
}


void Server::simpleInit(std::string const & servPath, std::string const & locationPath)
{
	MyConf conf;

	conf.parseConfig(servPath, locationPath);
	_config = conf;
	// _config.displayConfig();
}

vectLocationIt	Server::findLongestMatch(std::string const & text, vectLocation & location)
{
	int				nbMatch = 0;
	int				nbMatchMax = 0;
	std::string		compare;
	stringVect		vect1;
	stringVect		vect2;
	vectLocationIt	itFinal;

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
			// std::cout << "Compare " << vect1[i] << " | " << vect2[i] << std::endl;
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
	// std::cout << "++++++++++++++++++ nbMatchMax = " << nbMatchMax << std::endl;
	return itFinal;
}