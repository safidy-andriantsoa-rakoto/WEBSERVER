/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:00 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/23 15:46:01 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(Server const & server, const int &_clientFD, bool is_connect) : 
	Server(server),
	_clientFD(_clientFD),
	_connect(is_connect),
	_fileFD(NULL)
{
	this->_clientRequest= "";
	this->_to_send = 0;
	this->_total_send = 0;
	if (_clientFD < 0)
	{
		std::cout << "Error\nInvalid input FD" << std::endl; 
		return ;
	}
}

Client::Client(const Client &client) : Server(client)
{
	_fileFD = NULL;
	*this = client;
}

Client &Client::operator=(const Client &client)
{
	if (this != &client)
	{
		this->Server::operator=(client);
		this->_clientFD = client._clientFD;
		this->_connect = client._connect;
		this->_clientRequest = client._clientRequest;
		this->_client_stat = client._client_stat;
		this->_response = client._response;
		this->_to_send = client._to_send;
		this->_total_send = client._total_send;
		this->_boundary_sep = client._boundary_sep;
		this->_receivedSize = client._receivedSize;
		this->_client_stat = client._client_stat;
		this->_boundary_sep = client._boundary_sep;
		this->_request = client._request;
		this->_fileFD = client._fileFD;
	}
	return (*this);
}

Client::~Client(){}

size_t Client::getToSendSize()
{
    return (this->_to_send);
}

size_t Client::getTotalSendSize()
{
    return (this->_total_send);
}

bool Client::is_finishedSendContent()
{
	if (this->_total_send == this->_to_send)
		return (true);
    return (false);
}

Request &Client::getobjREquest()
{
    return (_request);
}

void Client::setToSendSize(size_t to_send)
{
	if (!to_send)
		this->_to_send = to_send;
}

void Client::IncrementTotalSendSize(size_t total_send)
{
	this->_total_send += total_send;
}

int Client::get_clientFD() const
{
	return (this->_clientFD);
}

void Client::setCatchedResponse(AExcept & e, int & fd, std::string & filePath)
{
	t_errPage	errPage = _config.getErrPage();
	_response.generateCatchedResponse(e, errPage, fd, filePath);
}

void Client::setStandardResponse(int const & statusCode)
{
	_response.setType(_fileFD->getFileType());
	_response.setStatusCode(statusCode);
	_response.generateStandardResponse(_fileFD->getBuffer());
}

void Client::setChunkedResponse(int const &statusCode)
{	
	_response.setStatusCode(statusCode);
	if (_fileFD)
	{
		_response.setType(_fileFD->getFileType());
		_response.generateChunkedResponse(_fileFD->getBuffer(), _fileFD->getStatus());
	}
}

void Client::ClientRequest()
{
	std::cout << "\033[045m			GET REQUEST " << this->_clientFD << "\033[0m" << std::endl;
	char response[_BUFFER_RECV];
	this->_receivedSize = recv(this->get_clientFD(), response, sizeof(response), 0);
	if (_receivedSize == 0)
		this->_connect = false;
	if (_receivedSize == -1)
		throw RecvError();
	this->_clientRequest = std::string(response, _receivedSize);
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

std::string Client::getBoundaryLimit() const
{
    return (this->_boundary_sep);
}

void Client::setFileFD(FileFd *fileFd)
{
	if (_fileFD)
		delete _fileFD;
	_fileFD = fileFd;
}

void Client::updateFileStatus(int const &status)
{
	_fileFD->setStatus(status);
}

void Client::increaseFileSent(size_t const &add)
{
	_fileFD->increaseFileSent(add);
}

Response & Client::getResponse()
{
	return (this->_response);
}

std::string Client::getFullResponse()
{
	return _response.getFullResponse();
}

std::string Client::getBodyResponse()
{
    return _response.getBody();
}

int Client::getCategoryResponse()
{
    return _response.getRespCategory();
}

int Client::getStatusCodeResponse()
{
    return _response.getStatusCode();
}

void Client::clearResponse()
{
	_response.clearResponse();
}

void Client::updateResponseCategory(int const &stat)
{
	_response.setCategory(stat);
}

FileFd *Client::getFileFD()
{
	return (_fileFD);
}

int Client::getFileStatus()
{
	return _fileFD->getStatus();
}

const char *Client::Deconnect::what() const throw()
{
	return ("disconnected!");
}

const char *Client::RecvError::what() const throw()
{
	return ("Error RECV\nget Request from client");
}


// void Client::processRequestWithConfig(Request & request)
void Client::processRequestWithConfig()
{
	checkContentLength();
	buildRequestTarget();
}

// void Client::checkContentLength(Request &request)
void Client::checkContentLength()
{
	float		maxSize;
	float		requestSize;
	stringMapIt	itFind;

	itFind = _request.getHeader().find("Content-Length");
	if (itFind == _request.getHeader().end())
		return ;
	maxSize = _config.getMaxSize();
	requestSize = convertStringToFLoat(_request.getHeader()["Content-Length"]);
	if (requestSize > maxSize)
		throw ErrorExcept(413, "Size limit not respected");
}

void Client::buildRequestTarget()
{
	vectLocationIt	it;
	std::string		target;
	vectLocation	loc;

	target = _request.getRequestTarget();
	std::cout << "\033[96m********TARGET = " << target << "***************\033[0m" << std::endl;
	if (target == "/")
	{
		processLocationOption(_config.getClassLocation()[0], _request);
		_request.setRequestTarget(_config.getClassLocation()[0].getRootDirectory() + "/" + "index.html");
		std::cout << "\033[96mREQUEST TARGET = " << _request.getRequestTarget() << "\033[0m" << std::endl;
		return ;
	}
	loc = _config.getClassLocation();
	it = findLongestMatch(target, loc);
	processLocationOption(*it, _request);
	std::cout << "\033[96mLOCATION SELECTED = " << (*it).getLocation() << " => " << (*it).getRootDirectory() << "\033[0m" << std::endl;
	if ((*it).getLocation() != "/")
		target.replace(target.find((*it).getLocation(), 0), (*it).getLocation().size(), (*it).getRootDirectory());
	else
		target = (*it).getRootDirectory() + target;
	_request.setRequestTarget(target);
	std::cout << "\033[96mREQUEST TARGET = " << _request.getRequestTarget() << "\033[0m" << std::endl;
}

void Client::processLocationOption(MyLoc const & location, Request & request)
{
	stringVect	locationMethod;

	locationMethod = location.getAvailableMethod();
	processRedirection(location, request);
	checkMethodInLocation(locationMethod, request.getMethod());
	if (location.getAutoindex() == "ON")
		request.setAutoIdx(true);
	else
		request.setAutoIdx(false);
}

void Client::checkMethodInLocation(stringVect & locationMethod, std::string const & method)
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
void Client::processRedirection(MyLoc const & location, Request & request)
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

int Client::get_request()
{
	int			fd;
	std::string	filePath;
	DIR*		dir;

	filePath = _request.getRequestTarget();
	std::cout << "Autoindex = " << _request.getAutoIdx() << std::endl;
	dir = opendir(filePath.c_str());
	if (!dir && errno == EACCES)
		throw ErrorExcept(403, "Directory forbidden");
	if (dir && _request.getAutoIdx() == true)
		throw LDirExcept(300, dir, filePath, "Listing directory");
	if (dir && _request.getAutoIdx() == false)
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

int Client::post_request(stringMap & header, std::string & md, std::string & boundary)
{
	int	fileFd;

	fileFd = open("storage", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fileFd== -1) {
		perror("open file in POST");
		throw ErrorExcept(404, "File can't be opened");
	}
	size_t is_exist =  header.find("Content-Type")->second.find(md);
	if (is_exist != std::string::npos)
	{
		std::string	getboundary = header.find("Content-Type")->second;
		size_t pos = getboundary.find(boundary);
		if (pos != std::string::npos)
		{
			this->_boundary_sep = getboundary.substr(pos + boundary.size());
		}
		std::cout << "|<" << this->_boundary_sep << ">" << std::endl;
		
		
		std::cout << "\nMULTIPART/FORM_DATA" << std::endl;
	}
	else
	{
		std::cout << "\nNORMAL DATA" << std::endl;
	}
	return fileFd;
}

void Client::delete_request(){}

int Client::processRequest(std::string const & clientRequest, uint32_t & opt, std::string & filePath)
{
	int			fileFd;
	Response	response;
    std::string responseString;
	t_errPage	errPage;
	std::string	type;
	stringMap	header;
	std::string md;
	std::string boundary;

	fileFd = -1;
	md = "multipart/form-data";
	boundary = "boundary=";
	_request.parseRequest(clientRequest);
	header = _request.getHeader();
	_request.displayRequest();
	processRequestWithConfig();
	if (_request.getMethod() == "GET")
	{
		fileFd = get_request();
		filePath = _request.getRequestTarget();
		type = Mime::getFileType(filePath); //Testing if type is listed on our default type list
		opt = EPOLLIN;
	}
	else if (_request.getMethod() == "POST")
	{
		fileFd = post_request(header, md, boundary);
		opt = EPOLLOUT;
	}
	else if (_request.getMethod() == "DELETE")
		std::cout << _request.getMethod() << std::endl; // Calling delete_request
	return fileFd;
}

vectLocationIt	Client::findLongestMatch(std::string const & text, vectLocation & location)
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