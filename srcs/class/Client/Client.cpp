/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:00 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/10/16 09:51:57 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(Server const & server, const int &_clientFD, bool is_connect) : 
	Server(server),
	_clientFD(_clientFD),
	_connect(is_connect),
	_cgiFD(NULL),
	_modPostFile(false)
{
	this->_clientRequest= "";
	this->_to_send = 0;
	this->_total_send = 0;
	if (_clientFD < 0)
		throw std::runtime_error("Client constructor error");
}

Client::Client(const Client &client) : Server(client)
{
	_cgiFD = NULL;
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
		setPtrCgiFd(client._cgiFD);
	}
	return (*this);
}

Client::~Client(){}

size_t Client::getToSendSize() {return (this->_to_send);}

size_t Client::getTotalSendSize() {return (this->_total_send);}

bool Client::is_finishedSendContent()
{
	if (this->_total_send == this->_to_send)
		return (true);
    return (false);
}

Request &Client::getObjRequest(){return (_request);}

void Client::setToSendSize(size_t to_send)
{
	if (!to_send)
		this->_to_send = to_send;
}

void Client::IncrementTotalSendSize(size_t total_send) {this->_total_send += total_send;}

int Client::get_clientFD() const {return (this->_clientFD);}

void Client::setCatchedResponse(AExcept & e)
{
	std::cout << "				Catched Response status code = " << e._statusCode << std::endl;
	t_errPage	errPage = _config.getErrPage();
	std::string	filePath = _request.getRequestTarget();
	_request.setIsCgi(false);
	_response.generateCatchedResponse(e, errPage, filePath, _fdType);
	_request.setRequestTarget(filePath);
}

void Client::setStandardResponse(std::string const & content)
{
	_response.generateStandardResponse(content);
}

void Client::setChunkedResponse(std::string const & content, std::string const & type, int const & status)
{	
	_response.setType(type);
	_response.generateChunkedResponse(content, status);
}

void Client::setCgiChunkedResponse()
{
	if (_cgiFD)
		_response.generateCgiChunkedResponse(_cgiFD->getBody(), _cgiFD->getHeader(), _cgiFD->getStatus());
}

void Client::setSingleResponse(int const &statusCode)
{
	_response.generateSingleResponse(statusCode);
}

void Client::receiveClientRequest()
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

bool Client::getConnexionStat(){return (this->_connect);}

std::string Client::getBoundaryLimit() const{return (this->_boundary_sep);}

void Client::setBoundaryLimit(std::string boundary){this->_boundary_sep = boundary;}

CgiFd *Client::getPtrCgiFd(){return _cgiFD;}

void Client::setPtrCgiFd(CgiFd *cgiFd)
{
	clearPtrCgiFd();
	_cgiFD = cgiFd;
}

void Client::clearPtrCgiFd()
{
	if (_cgiFD)
		delete _cgiFD;
	_cgiFD = NULL;
}

int Client::getCgiFdStatus(){return _cgiFD->getStatus();}

void Client::updateCgiFdStatus(int const &status){_cgiFD->setStatus(status);}

Response & Client::getObjResponse(){return (this->_response);}

std::string Client::getFullResponse(){return _response.getFullResponse();}

std::string Client::getBodyResponse(){return _response.getBody();}

int Client::getCategoryResponse(){return _response.getRespCategory();}

int Client::getStatusCodeResponse(){return _response.getStatusCode();}

void Client::setStatusCodeResponse(int const &status){_response.setStatusCode(status);}

void Client::clearResponse(){_response.clearResponse();}

void Client::updateResponseCategory(int const &stat){_response.setCategory(stat);}

int Client::getFdType() const{return _fdType;}

void Client::setFdType(int const &type){_fdType = type;}

const char *Client::Deconnect::what() const throw()
{
	return ("disconnected!");
}

const char *Client::RecvError::what() const throw()
{
	return ("Error RECV\nget Request from client");
}
/*
	If method == POST or DELETE
	Find "upload" in location or add upload (default) location : Config file (parsing) => TELL TSIKY !!!
	set request target to root
*/
void Client::processRequestWithConfig()
{
	vectLocationIt	it;
	std::string		target = _request.getRequestTarget();

	checkContentLength(); //See with POST method [Safidy]
	if (target.find(".php", 0) != std::string::npos)
	{
		_request.clearCgiClass();
		_request.setIsCgi(true);
		it = _config.findClassLocation(".php");
		processLocationOption(*it, _request);
		_request.processCgiClass(target, (*it).getRootDirectory());
		target = (*it).getRootDirectory() + target; //Review if request target needed ? normaly done during parsing Cgi
		_request.setRequestTarget(target);
		return;
	}
	if (_request.getMethod() == "POST" || _request.getMethod() == "DELETE")
	{
		it = _config.findClassLocation("/upload");
		std::string	newTarget = (*it).getRootDirectory();

		if (_request.getMethod() == "DELETE")
			newTarget += target;
		_request.setRequestTarget(newTarget);
		return;
	}
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
		it = _config.findClassLocation("/");
		if ((*it).getAutoindex() == "ON")
		{
			_request.setRequestTarget(_config.getClassLocation()[0].getRootDirectory());
		}
		else
		{
			_request.setRequestTarget(_config.getClassLocation()[0].getRootDirectory() + "/" + "index.html");
		}
		std::cout << "\033[96mREQUEST TARGET = " << _request.getRequestTarget() << "\033[0m" << std::endl;
		return ;
	}
	loc = _config.getClassLocation();
	if (target.find(".php", 0) != std::string::npos)
	{
		_request.clearCgiClass();
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

void Client::get_request(uint32_t & opt, int *fd, std::string & content)
{
	if (_request.getIsCgi() == false)
	{
		_fdType = HTML_FILE;
		stdGetRequest(content);
		opt = EPOLLOUT;
	}
	else
	{
		_fdType = GET_PHP_SCRIPT;
		cgiGetRequest(fd);
		opt = EPOLLIN;
	}
	setStatusCodeResponse(200);
}

void Client::stdGetRequest(std::string & content)
{
	std::string	filePath;
	DIR*		dir;

	filePath = _request.getRequestTarget();
	dir = opendir(filePath.c_str());
	if (!dir && errno == EACCES)
		throw ErrorExcept(403, "Directory forbidden");
	if (dir && _request.getAutoIdx() == true)
		throw LDirExcept(300, dir, _request.getOriginalRequestTarget(), "Listing directory");
	if (dir && _request.getAutoIdx() == false)
	{
		filePath += "/index.html";
		closedir(dir);
	}
	try
	{
		content = readFileToString(filePath);
	}
	catch(const std::exception& e)
	{
		throw ErrorExcept(404, "File not found");
	}
}

void Client::cgiGetRequest(int *fd)
{
	int	pipeFd[2];
	int	pid;
	int	errFd[2];

	if (pipe(pipeFd) == -1)
		throw std::runtime_error("Pipe fd error");
	if (pipe(errFd) == -1)
		throw std::runtime_error("Pipe err error");
	std::cout << "Pipe_fd [0] = " << pipeFd[0] << " [1] = " << pipeFd[1] << std::endl;
	std::cout << "errFd [0] = " << errFd[0] << " [1] = " << errFd[1] << std::endl;
	pid = fork();
	if (pid == -1)
		throw std::runtime_error("Fork error");
	if (pid == 0)
	{
		dup2(errFd[1], STDERR_FILENO);
		close(errFd[0]);
		close(errFd[1]);
		dup2(pipeFd[1], STDOUT_FILENO);
		close(pipeFd[0]);
		close(pipeFd[1]);
		try
		{
			_request.execCgiClass();
		}
		catch(const std::exception& e)
		{
			fd[FD_PRINCIPAL] = -2;
			fd[FD_STDERR] = -2;
		}
	}
	else
	{
		close(errFd[1]);
		close(pipeFd[1]);
		_request.clearCgiClass();
		fd[FD_PRINCIPAL] = pipeFd[0];
		fd[FD_STDERR] = errFd[0];
	}
}

void Client::post_request(stringMap &header, std::string &boundary, uint32_t & opt, int *fd)
{
	if (_request.getIsCgi() == false)
	{
		_fdType = HTML_FILE;
		stdPostRequest(header, boundary, opt, fd);
		setSingleResponse(201);
	}
	else
	{
		opt = EPOLLIN;
		_fdType = POST_PHP_SCRIPT;
		cgiPostRequest(fd);
		setStatusCodeResponse(200);
	}
}

void Client::stdPostRequest(stringMap &header, std::string &boundary, uint32_t & opt, int *fd)
{
	int	fdPost = -1;
	std::string	fullPath;

	if (header.find("Content-Type")->second.find("multipart/form-data") != std::string::npos)
	{
		// if (!this->_fileFD)
		// {
			fullPath = _request.getRequestTarget() + "/" +  "storage_multi_form_data";

			fdPost = open(fullPath.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fdPost== -1) {
				perror("open file in POST");
				throw ErrorExcept(404, "File can't be opened");
			}
			opt = EPOLLOUT;
			std::cout << "FD FILE->POST Multipart/data=" << fdPost << std::endl; 
		// }
		// else
		// 	fdPost = -1;
		std::string	getboundary = header.find("Content-Type")->second;
		size_t pos = getboundary.find(boundary);
		if (pos != std::string::npos)
			this->_boundary_sep = getboundary.substr(pos + boundary.size());
		// std::cout << "|<" << this->_boundary_sep << ">" << std::endl;
		std::cout << "\nMULTIPART/FORM_DATA" << std::endl;
		fdPost = -1;
	}
	else if (header.find("Content-Type")->second.find("application/x-www-form-urlencoded") != std::string::npos)
	{
		std::cout << "\napplication/x-www-form-urlencoded\n" << std::endl;
		// if (!_fileFD)
		// {
			fullPath = _request.getRequestTarget() + "/" +  "storage_app_xwfurlencoded";
			fdPost = open(fullPath.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fdPost== -1) {
				perror("open file in POST");
				throw ErrorExcept(404, "File can't be opened");
			}
			opt = EPOLLOUT;
			std::cout << "FD FILE->POST APP/x-wwwfurlencode=" << fdPost << std::endl; 
		// }
		// fdPost = -1;
	}
	else if (header.find("Content-Type")->second.find("text/plain") != std::string::npos)
	{
		if (_request.get_Body().find("\r\n\r\n") != std::string::npos)
		{
			std::cout << "			VITA ZANY EEE" << std::endl;	
			_request.set_is_CRLF_header(false);
		}
		std::cout << "\ntext/plain\n" << std::endl;
		// if (!_fileFD)
		// {
			fullPath = _request.getRequestTarget() + "/" +  "storage_text_plain";
			fdPost = open(fullPath.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fdPost== -1) {
				perror("open file in POST");
				throw ErrorExcept(404, "File can't be opened");
			}
			opt = EPOLLOUT;
			std::cout << "FD FILE->POST text/plain=" << fdPost << std::endl; 
		// }
		// fdPost = -1;
	}
	else
		std::cout << "          OAY EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << std::endl;
	opt = EPOLLOUT;
	fd[FD_PRINCIPAL] = fdPost;
}

void Client::cgiPostRequest(int *fd)
{
	int	pid;
	int	outFd[2];
	int	inFd[2];
	int	errFd[2];

	if (pipe(inFd) == -1)
		throw std::runtime_error("input fd error");
	if (pipe(outFd) == -1)
		throw std::runtime_error("output fd error");
	if (pipe(errFd) == -1)
		throw std::runtime_error("error fd error");
	std::cout << "inFd [0] = " << inFd[0] << " [1] = " << inFd[1] << std::endl;
	std::cout << "outFd [0] = " << outFd[0] << " [1] = " << outFd[1] << std::endl;
	std::cout << "errFd [0] = " << errFd[0] << " [1] = " << errFd[1] << std::endl;
	pid = fork();
	if (pid == -1)
		throw std::runtime_error("Fork error");
	if (pid == 0)
	{
		dup2(inFd[0], STDIN_FILENO);
		close(inFd[0]);
		close(inFd[1]);
		dup2(outFd[1], STDOUT_FILENO);
		close(outFd[0]);
		close(outFd[1]);
		dup2(errFd[1], STDERR_FILENO);
		close(errFd[0]);
		close(errFd[1]);
		try
		{
			_request.execCgiClass();
		}
		catch(const std::exception& e)
		{
			std::cout << "		THERE WAS AN ERROR!!!!!!!!!\n";
			fd[FD_STDIN] = -2;
			fd[FD_PRINCIPAL] = -2;
			fd[FD_STDERR] = -2;
		}
	}
	else
	{
		close(inFd[0]);
		close(outFd[1]);
		close(errFd[1]);
		_request.clearCgiClass();
		fd[FD_STDIN] = inFd[1];
		fd[FD_PRINCIPAL] = outFd[0];
		fd[FD_STDERR] = errFd[0];
	}
}

void Client::delete_request()
{
	// std::string		root;
	// std::string		target;
	// stringVectIt	_configIt;
	// vectLocation	loc;

	// target = _request.getRequestTarget();
	// loc = _config.getClassLocation();
	// if (!target.size())
	// 	throw ErrorExcept(403, "Forbidden");
	// if (loc.size())
	// {
	// 	root = loc[0].getRootDirectory();
	// 	if (target.compare(0, root.size(), root) != 0)
	// 		throw ErrorExcept(401, "Unauthorized");
	// 	struct  stat s_stat;
	// 	if (stat(target.c_str(), &s_stat) != 0)
	// 		throw ErrorExcept(401, "Unauthorized");
	// 	if (S_ISREG(s_stat.st_mode))
	// 	{
	// 		if (std::remove(target.c_str()) != 0)
	// 			throw ErrorExcept(401, "Unauthorized");
	// 	}
	// 	else
	// 		throw ErrorExcept(403, "Forbidden");
	// }

	/*	Check error for delete */
	std::string	target;
	struct stat	s_stat;

	target = _request.getRequestTarget();
	if (!target.size())
		throw ErrorExcept(403, "Forbidden");
	if (stat(target.c_str(), &s_stat) != 0)
		throw ErrorExcept(401, "Unauthorized");
	if (S_ISREG(s_stat.st_mode))
	{
		if (std::remove(target.c_str()) != 0)
			throw ErrorExcept(401, "Unauthorized");
	}
	else
		throw ErrorExcept(403, "Forbidden");
	_response.setCategory(SINGLE_RSP);
}

void Client::processRequest(std::string const & clientRequest, uint32_t & opt, int *fd, std::string & content)
{
	stringMap	header;
	std::string boundary;

	boundary = "boundary=";
	if (!_request.get_is_CRLF_header())
	{
		_request.parseRequest(clientRequest);
		processRequestWithConfig();
	}
	else
		_request.set_Body(clientRequest);
	header = _request.getHeader();
	_request.displayRequest();
	if (_request.getMethod() == "GET")
		get_request(opt, fd, content);
	else if (_request.getMethod() == "POST")
	{
		post_request(header, boundary, opt, fd);
	}
	else if (_request.getMethod() == "DELETE")
	{
		delete_request();
		setSingleResponse(204);
	}
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

int Client::creat_file_for_post(std::string path, uint32_t & opt)
{
	int fd = -1;
	if (path.size())
	{
		fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			throw ErrorExcept(404, "File can't be opened");
		opt = EPOLLOUT;
	}
	return (fd);
}

std::string Client::currentDateTimeString()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[15];

	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tstruct);
	return (buf);
}

void Client::switch_modPostFile(bool stat){this->_modPostFile = stat;}
