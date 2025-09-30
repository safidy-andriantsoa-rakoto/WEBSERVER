/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:00 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/30 14:52:15 by barandri         ###   ########.fr       */
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
		throw std::runtime_error("Client constructor error");
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
		setFileFD(client._fileFD);
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
	char response[_BUFFER_RECV];

	std::cout << "\033[045m			GETTING REQUEST FROM FD = " << this->_clientFD << "\033[0m" << std::endl;
	this->_receivedSize = recv(this->get_clientFD(), response, sizeof(response), 0);
	if (_receivedSize == 0)
		this->_connect = false;
	if (_receivedSize == -1)
	{
		perror("ClientRequest - Recv");
		clearAllFd();
		throw RecvError();
	}
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
	clearFileFd();
	_fileFD = fileFd;
}

void Client::clearFileFd()
{
	if (_fileFD)
		delete _fileFD;
	_fileFD = NULL;
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

void Client::processRequestWithConfig()
{
	checkContentLength(); //See with POST method [Safidy]
	buildRequestTarget();
}

void Client::checkContentLength()
{
	float		maxSize;
	float		requestSize;
	stringMapIt	itFind;

	maxSize = 0;
	requestSize = 0;
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
	_request.setIsCgi(false);
	std::cout << "\033[96m********TARGET = " << target << "***************\033[0m" << std::endl;
	if (target == "/")
	{
		processLocationOption(_config.getClassLocation()[0], _request);
		_request.setRequestTarget(_config.getClassLocation()[0].getRootDirectory() + "/" + "index.html");
		std::cout << "\033[96mREQUEST TARGET = " << _request.getRequestTarget() << "\033[0m" << std::endl;
		return ;
	}
	loc = _config.getClassLocation();
	if (target.find(".php", 0) != std::string::npos)
	{
		_request.clearCgiClass();
		std::cout << "CGI ON GOING !!!!" << std::endl;
		_request.setIsCgi(true);
		it = _config.findClassLocation(".php");
		processLocationOption(*it, _request);
		_request.processCgiClass(target, (*it).getRootDirectory());
		target = (*it).getRootDirectory() + target; //Review if request target needed ? normally done during parsing Cgi
	}
	else
	{
		it = findLongestMatch(target, loc);
		processLocationOption(*it, _request);
		std::cout << "\033[96mLOCATION SELECTED = " << (*it).getLocation() << " => " << (*it).getRootDirectory() << "\033[0m" << std::endl;
		if ((*it).getLocation() != "/")
			target.replace(target.find((*it).getLocation(), 0), (*it).getLocation().size(), (*it).getRootDirectory());
		else
			target = (*it).getRootDirectory() + target;
	}
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

/*
	Review redirection function [Barandri]
*/
void Client::processRedirection(MyLoc const & location, Request & request)
{
	std::string	oldUri;
	std::string	newUri;
	stringVect	redirect;
	int			statusRedirect;

	redirect = location.getRedirect();
	if (!redirect.size())
		return ;
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
	if (_request.getIsCgi() == false)
		return stdGetRequest();
	else
		return cgiGetRequest();
}

int Client::stdGetRequest()
{
	int			fd;
	std::string	filePath;
	DIR*		dir;

	filePath = _request.getRequestTarget();
	dir = opendir(filePath.c_str());
	if (!dir && errno == EACCES)
		throw ErrorExcept(403, "Directory forbidden");
	if (dir && _request.getAutoIdx() == true)
		throw LDirExcept(300, dir, _request.getOriginalRequestTarget(), "Listing directory");
	if (dir && _request.getAutoIdx() == false)
	if (dir && _request.getAutoIdx() == false)
	{
		filePath += "/index.html";
		closedir(dir);
	}		
	fd = open(filePath.c_str(), O_RDONLY | O_CLOEXEC);
	std::cout << "\033[046mSTD FD=" << fd << "\033[0m" << std::endl;
	if (fd == -1)
	{
		perror("Open file");
		if (errno == EACCES)
			throw ErrorExcept(403, "No permission for file");
		throw ErrorExcept(404, "File can't be opened");
	}
	return fd;
}

int Client::cgiGetRequest()
{
	int	fd;

	/*
		create pipe => pipe_fd[2]
		fork()
			---child
				clean every unecessary fd inside epoll and close, or any allocation
				how to clean envp???
				execute _cgi from request;
			---parent
				return pipe_fd[0] because read with get to add it inside the epoll_event;
				sSee what happen with child if ctrl+c?
	*/
    return fd;
}

int Client::post_request(stringMap &header, std::string &boundary, uint32_t & opt)
{
	int	fd;

	if (header.find("Content-Type")->second.find("multipart/form-data") != std::string::npos)
	{
		if (!this->_fileFD)
		{
			fd = open("storage_multi_form_data", O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd== -1) {
				perror("open file in POST");
				throw ErrorExcept(404, "File can't be opened");
			}
			opt = EPOLLOUT;
			std::cout << "FD FILE->POST Multipart/data=" << fd << std::endl; 
		}
		else
			fd = -1;
		std::string	getboundary = header.find("Content-Type")->second;
		size_t pos = getboundary.find(boundary);
		if (pos != std::string::npos)
			this->_boundary_sep = getboundary.substr(pos + boundary.size());
		// std::cout << "|<" << this->_boundary_sep << ">" << std::endl;
		std::cout << "\nMULTIPART/FORM_DATA" << std::endl;
		fd = -1;
	}
	else if (header.find("Content-Type")->second.find("application/x-www-form-urlencoded") != std::string::npos)
	{
		std::cout << "\napplication/x-www-form-urlencoded\n" << std::endl;
		if (!_fileFD)
		{
			fd = open("storage_app_xwfurlencoded", O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd== -1) {
				perror("open file in POST");
				throw ErrorExcept(404, "File can't be opened");
			}
			opt = EPOLLOUT;
			std::cout << "FD FILE->POST APP/x-wwwfurlencode=" << fd << std::endl; 
		}
		fd = -1;
	}
	else if (header.find("Content-Type")->second.find("text/plain") != std::string::npos)
	{
		if (_request.get_Body().find("\r\n\r\n") != std::string::npos)
		{
			std::cout << "			VITA ZANY EEE" << std::endl;	
			_request.set_is_CRLF_header(false);
		}
		std::cout << "\ntext/plain\n" << std::endl;
		if (!_fileFD)
		{
			fd = open("storage_text_plain", O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd== -1) {
				perror("open file in POST");
				throw ErrorExcept(404, "File can't be opened");
			}
			opt = EPOLLOUT;
			std::cout << "FD FILE->POST text/plain=" << fd << std::endl; 
		}
		fd = -1;
	}
	else
		std::cout << "          OAY EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << std::endl;
	opt = EPOLLOUT;
	return fd;
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
	// std::string md;
	std::string boundary;

	fileFd = -1;
	// md = "";
	boundary = "boundary=";
	
	if (!_request.get_is_CRLF_header())
	{
		_request.parseRequest(clientRequest);
		processRequestWithConfig();
	}
	else
	{
		_request.set_Body(clientRequest);
	}
	header = _request.getHeader();
	/////////////////////////////////////
	_request.displayRequest();
	if (_request.getMethod() == "GET")
	{
		fileFd = get_request();
		filePath = _request.getRequestTarget();
		type = Mime::getFileType(filePath); //Testing if type is listed on our default type list
		opt = EPOLLIN;
	}
	else if (_request.getMethod() == "POST")
	{
		fileFd = post_request(header, boundary, opt);
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