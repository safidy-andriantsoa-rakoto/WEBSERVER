/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:55 by barandri          #+#    #+#             */
/*   Updated: 2025/09/03 19:35:21 by rsafidy-         ###   ########.fr       */
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

	itFind = request.getHeader().find("Content-Length");
	if (itFind == request.getHeader().end())
		return ;
	maxSize = _config.getMaxSize();
	requestSize = convertStringToFLoat(request.getHeader()["Content-Length"]);
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
	// if (_fdClient != -1)
	// 	close(_fdClient);
	// if (_fdSocket != -1)
	// 	close(_fdSocket);
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
	int fd = accept(_fdSocket, (sockaddr *)&(this->_sockaddrIn), (socklen_t *)&(this->_size_sockaddrin));
	if (this->_fdClient == -1)
	{
		std::cout << "Error\naccept a connection on a socket" << std::endl;
		return (-2);
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

void Server:: processRequest(std::string const & clientRequest, int const & fd_cli)
{
	Request		request;
	Response	response;
    std::string responseString;
	t_errPage	errPage;
	/************************* */
	stringVect path;
    DIR* dir = opendir("ext");
	if (!dir)
	{
		std::cout << "Error\n dirent" << std::endl;
		return ;
	}
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
	{
		std::string name(entry->d_name);
		if (!name.compare(".") || !name.compare(".."))
			continue;	
		path.push_back("./ext/" + name);
    }
    closedir(dir);
	Mime test(path);
	/************************ */
	if (!clientRequest.length())
		return ;
	try
	{
		request.parseRequest(clientRequest);
	}
	catch(const std::exception & e)
	{
		//msg from client error
		return;
	}
	try
	{
		processRequestWithConfig(request);
		if (request.getMethod() == "GET")
			response = get_request(request, test.get_mime());
		else if (request.getMethod() == "POST")
			std::cout << request.getMethod() << std::endl; // Calling post_request
		else if (request.getMethod() == "DELETE")
			std::cout << request.getMethod() << std::endl; // Calling delete_request
	}
	catch(AExcept & e)
	{
		std::cerr << "Process Request exception catched : " << e._errMsg << std::endl; //Fafana
		errPage = _config.getErrPage();
		response.generateCatchedResponse(e, errPage);
	}
	responseString = response.getFullResponse();
	if (responseString.size())
		send_value(fd_cli, responseString);
	else
		std::cout << "ERROR/nresponce null" << std::endl;
}

Response Server::get_request(Request & request, MapMime mime)
{
	std::string		line;
	std::string 	tmp_ext;
	std::string 	ext_file;
	std::string 	type;
	std::fstream	file;
	MapMime::iterator it;
	struct stat st;
	
	tmp_ext = request.getRequestTarget();
	stat(tmp_ext.c_str(), &st); //permet d'obtenir extement la taille du fichier !important
	request.setRequestSize(st.st_size);

	std::cout << "			TAILLE DE LA FICHIER EXACT=" << (request.getRequestSize()) << std::endl;

	//Use of config file parameter to search for the file!!!
	//On error : need to specify the statusCode (no need to throw?)
	file.open(tmp_ext.c_str());
	if (!file.is_open())
		throw ErrorExcept(404, "File unreachable");
	//Calling a generic function to get type based on file extension
	ext_file = getFileExtension(tmp_ext);
	it = mime.find(ext_file);
	if (ext_file == "/")
		type = "text/html";
	else if (it != mime.end())
		type = it->second;
	Response response(200, type);
	response.generateSuccessResponse(request.getRequestTarget(), request.getRequestSize());
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

void Server::simpleInit(std::string const & servPath, std::string const & locationPath)
{
	MyConf conf;

	conf.parseConfig(servPath, locationPath);
	_config = conf;
	// _config.displayConfig();
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