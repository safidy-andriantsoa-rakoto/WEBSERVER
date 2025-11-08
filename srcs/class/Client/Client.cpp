/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:00 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/11/08 13:52:31 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(Server const & server, const int &_clientFD, bool is_connect) : 
	Server(server),
	_receivedSize(0),
	_clientFD(_clientFD),
	_connect(is_connect),
	_to_send(0),
	_total_send(0),
	_cgiFD(NULL),
	_fdType(-1),
	_modPostFile(false),
	_time_now(time(NULL)),
	_last_time(_time_now)
{
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
		this->_receivedSize = client._receivedSize;
		this->_client_stat = client._client_stat;
		this->_clientFD = client._clientFD;
		this->_connect = client._connect;
		this->_clientRequest = client._clientRequest;
		this->_boundary_sep = client._boundary_sep;
		this->_to_send = client._to_send;
		this->_total_send = client._total_send;
		this->_request = client._request;
		this->_response = client._response;
		setPtrCgiFd(client._cgiFD);
		this->_fdType = client._fdType;
		this->_filepath = client._filepath;
		this->_modPostFile = client._modPostFile;
		this->_time_now = client._time_now;
		this->_last_time = client._last_time;
	}
	return (*this);
}

Client::~Client(){}

size_t Client::getToSendSize(){return (this->_to_send);}

size_t Client::getTotalSendSize(){return (this->_total_send);}

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

void Client::closeRspFd(){_response.closeRegFileFd();}

void Client::setRspRegFileFd(int const & fd)
{
	_response.setRegFileFd(fd);
}

void Client::setRspContentLength(size_t const &size){_response.setContentLength(size);}

void Client::setRegStandardResponse(int const & fd, int const & stat)
{
	_response.setRegFileFd(fd);
	_response.setRespStat(stat);
	if (stat == OP_ONCE)
		_response.generateRegStandardResponse();
}

void Client::setRegFdChunkedResponse()
{
	std::cout << "\033[35mSending reg chunked response" << "\033[0m"<< std::endl;
	_response.generateRegChunkedResponse();
}

void Client::setStringSingleResponse(std::string const & content){_response.generateStringSingleResponse(content);}

void Client::setStringChunkedResponse()
{
	std::string content;
	size_t		pos;

	std::cout << "\033[35mSending string chunked response" << "\033[0m"<< std::endl;
	if (_response.getRespStat() == OP_START)
		_response.setPosition(0);
	pos = _response.getPosition();
	content = divideStringBySize(_response.getOriginBody(), pos, _BUFFER);
	pos += _BUFFER;
	if (pos > _response.getOriginBody().size())
	{
		_response.setRespStat(OP_CLOSE);
		pos = 0;
	}
	_response.generateStringChunkedResponse(content);
	_response.setPosition(pos);
}

void Client::setupCgiResponse()
{
	size_t	size;

	setStatusCodeResponse(_cgiFD->getStatusCode());
	setRspType(_cgiFD->getContentType(), 1);
	size = _cgiFD->getBody().size();
	if (size <= _BUFFER)
	{
		setResponseStat(OP_ONCE);
		setStringSingleResponse(_cgiFD->getBody());
	}
	else
	{
		setResponseStat(OP_START);
		getObjResponse().setOriginBody(_cgiFD->getBody());
	}
	_cgiFD->setRespMode(true);
}

void Client::setSingleResponse(int const &statusCode){_response.generateSingleResponse(statusCode);}

void Client::receiveClientRequest()
{
	char response[_BUFFER_RECV];

	std::cout << "\033[045m			GETTING REQUEST FROM FD = " << this->_clientFD << "\033[0m" << std::endl;
	set_last_time_activity();
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

int Client::getCgiFdStatus(){return _cgiFD->getReadStat();}

void Client::updateCgiFdStatus(int const &status){_cgiFD->setReadStat(status);}

Response & Client::getObjResponse(){return (this->_response);}

std::string Client::getFullResponse(){return _response.getFullResponse();}

std::string Client::getBodyResponse(){return _response.getBody();}

int Client::getRspCategory(){return _response.getRespCategory();}

int Client::getStatusCodeResponse(){return _response.getStatusCode();}

void Client::setStatusCodeResponse(int const &status){_response.setStatusCode(status);}

void Client::clearResponse(){_response.clearResponse();}

void Client::setRspCategory(int const &stat){_response.setRespCategory(stat);}

int Client::getFdType() const{return _fdType;}

void Client::setFdType(int const &type){_fdType = type;}

int Client::getResponseStat() const{return _response.getRespStat();}

void Client::setResponseStat(int const &stat){_response.setRespStat(stat);}

std::string Client::getRspOriginPath() const{return _response.getOriginPath();}

void Client::setRspOriginPath(std::string const &path){_response.setOriginPath(path);}

void Client::setRspType(std::string const &filePath){_response.setType(Mime::getFileType(filePath));}
void Client::setRspType(std::string const & type, int){_response.setType(type);}

const char *Client::Deconnect::what() const throw(){return ("disconnected!");}

const char *Client::RecvError::what() const throw(){return ("Error RECV\nget Request from client");}

void Client::processRequestWithConfig()
{
	vectLocationIt	it;
	std::string		target = _request.getRequestTarget();

	if (_request.getHttp() != "HTTP/1.1")
		throw ErrorExcept(505, "This server only support HTTP/1.1\n", "Http version error");
	_request.setIsCgi(false);
	buildRequestTarget();
	if (_request.getIsCgi() == false && (_request.getMethod() == "POST" || _request.getMethod() == "DELETE"))
	{
		checkContentLength();
		it = _config.findClassLocation("/upload");
		std::string	newTarget = (*it).getRootDirectory();
		if (_request.getMethod() == "DELETE")
			newTarget += target;
		_request.setRequestTarget(newTarget);
	}
}

void Client::checkContentLength()
{
	float		maxSize;
	float		requestSize;
	stringMapIt	itFind;

	maxSize = 0;
	requestSize = 0;
	if (_request.getMethod() != "POST")
		return ;
	itFind = _request.getHeader().find("Content-Length");
	if (itFind == _request.getHeader().end())
		throw ErrorExcept(411, "Length required");
	maxSize = _config.getMaxSize();
	requestSize = convertStringToFLoat(_request.getHeader()["Content-Length"]);
	if (requestSize > maxSize)
		throw ErrorExcept(414, "Size limit not respected");
}

void Client::buildRequestTarget()
{
	vectLocationIt	itLoc;
	std::string		target;
	std::string		originTarget;
	vectLocation	loc;

	target = _request.getRequestTarget();
	originTarget = target;
	std::cout << "\033[96m********TARGET = " << target << "***************\033[0m" << std::endl;
	if (target == "/")
	{
		processLocationOption(_config.getClassLocation()[0], _request);
		itLoc = _config.findClassLocation("/");
		target = _config.getClassLocation()[0].getRootDirectory();
		target += "/index.html";
	}
	else
	{
		loc = _config.getClassLocation();
		itLoc = findLongestMatch(target, loc);
		processLocationOption(*itLoc, _request);
		std::cout << "\033[96mLOCATION SELECTED = " << (*itLoc).getName() << " => " << (*itLoc).getRootDirectory() << "\033[0m" << std::endl;
		if ((*itLoc).getName() != "/")
			target.replace(target.find((*itLoc).getName(), 0), (*itLoc).getName().size(), (*itLoc).getRootDirectory());
		else
			target = (*itLoc).getRootDirectory() + target;
	}
	_request.setRequestTarget(target);
	std::cout << "\033[96mREQUEST TARGET = " << _request.getRequestTarget() << "\033[0m" << std::endl;
	if (CgiParser::checkIfFileCgi(target) == true)
	{
		std::cout << "\033[42m" << "CGI ON GOING" << "\033[0m" << std::endl;
		if ((*itLoc).getCgiActivation() == "OFF")
			throw ErrorExcept(403, "Cgi extension is OFF");
		_request.processCgiClass(target, (*itLoc).getCgiExecPath());
	}
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

void Client::processRedirection(MyLoc const & location, Request & request)
{
	std::string	oldUri;
	std::string	newUri;
	stringVect	redirect;
	int			statusRedirect;

	/* [!] : Review redirection process*/
	redirect = location.getRedirect();
	if (!redirect.size())
		return ;
	redirect = location.getRedirect();
	statusRedirect = convertStringToInt(redirect[0]);
	newUri = redirect[1];
	if (newUri.find("$uri") != std::string::npos)
	{
		oldUri = request.getRequestTarget();
		oldUri.erase(oldUri.find(location.getName()), location.getName().size() + 1);
		replaceString(newUri, oldUri, "$uri");
	}
	throw RedirExcept(statusRedirect, "Redirection", newUri);
}

void Client::get_request(uint32_t & opt, int *fd, pid_t & childPid)
{
	if (_request.getIsCgi() == false)
	{
		setRspCategory(STD_RSP);
		_fdType = STD_FILE;
		stdGetRequest();
		opt = EPOLLOUT;
	}
	else
	{
		setRspCategory(CGI_RSP);
		_fdType = GET_PHP_SCRIPT;
		cgiGetRequest(fd, childPid);
		setStatusCodeResponse(200);
		opt = EPOLLIN;
	}
}

void Client::stdGetRequest()
{
	std::string	filePath;
	std::string	fileContent;
	DIR*		dir;

	filePath = _request.getRequestTarget();
	if (checkIfDirectoryAvailable(filePath) == true)
	{
		if (_request.getAutoIdx() == true)
		{
			dir = opendir(filePath.c_str());
			throw LDirExcept(200, dir, _request.getOriginalRequestTarget(), "Listing directory");
		}
		filePath += "/index.html";
	}
	int fd = openRegFile(filePath);
	size_t	fileSize = getFileSize(filePath);
	int		stat;
	setStatusCodeResponse(200);
	setRspOriginPath(filePath);
	setRspType(filePath);
	setRspContentLength(fileSize);
	if (fileSize <= _BUFFER)
		stat = OP_ONCE;
	else
		stat = OP_START;
	setRegStandardResponse(fd, stat);
}

void Client::cgiGetRequest(int *fd, pid_t & childPid)
{
	int		pipeFd[2];
	int		stockStdOut;

	if (pipe(pipeFd) == -1)
		throw ErrorExcept(500, "Pipe fd error");
	childPid = fork();
	if (childPid == -1)
		throw ErrorExcept(500, "Fork error");
	if (childPid == 0)
	{
		stockStdOut = dup(STDOUT_FILENO);
		makeDup2(pipeFd[1], STDOUT_FILENO, pipeFd[0]);
		try
		{
			_request.execCgiClass();
		}
		catch(const std::exception& e)
		{
			makeDup2(stockStdOut, STDOUT_FILENO);
			_request.clearCgiClass(); /* [?] : Needed for leaks? */
			fd[FD_STDOUT] = -2;
			throw std::runtime_error("Execve error");
		}
	}
	else
	{
		close(pipeFd[1]);
		_request.clearCgiClass();
		fd[FD_STDOUT] = pipeFd[0];
	}
}

void Client::post_request(stringMap &header, uint32_t & opt, int *fd, pid_t & childPid)
{
	if (_request.getIsCgi() == false)
	{
		setRspCategory(STD_RSP);
		_fdType = STD_FILE;
		stdPostRequest(header, opt);
	}
	else
	{
		setRspCategory(CGI_RSP);
		opt = EPOLLIN;
		_fdType = POST_PHP_SCRIPT;
		cgiPostRequest(fd, childPid);
		setStatusCodeResponse(200);
	}
}

void Client::stdPostRequest(stringMap &header, uint32_t & opt)
{
	opt = EPOLLIN;

	if (header.find("Content-Length") == header.end())
		throw ErrorExcept(411, "Length required");
	if (header.find("Content-Type")->second.find("multipart/form-data") != std::string::npos)
	{
		get_boundary_from_header(header);
		process_multipart_data();
		if (_request.get_c_length_received() == _request.get_content_length())
			setSingleResponse(201);
	}
	else if (header.find("Content-Type")->second.find("application/x-www-form-urlencoded") != std::string::npos)
	{
		if (_request.get_c_length_received() == _request.get_content_length())
			setSingleResponse(200);
	}
	else if (header.find("Content-Type")->second.find("text/plain") != std::string::npos)
	{
		if (_request.get_c_length_received() == _request.get_content_length())
			setSingleResponse(200);
	}
	else
		throw ErrorExcept(501, "Not implemented");
}

void Client::cgiPostRequest(int *fd, pid_t & childPid)
{
	int	pipeOutFd[2];
	int	pipeInFd[2];
	int	stdFd[2] = {-1, -1};

	if (pipe(pipeInFd) == -1)
		throw ErrorExcept(500, "input fd error");
	if (pipe(pipeOutFd) == -1)
		throw ErrorExcept(500, "output fd error");
	childPid = fork();
	if (childPid == -1)
		throw ErrorExcept(500, "Fork error");
	if (childPid == 0)
	{
		dupStandardIO(stdFd);
		makeDup2(pipeInFd[0], STDIN_FILENO, pipeInFd[1]);
		makeDup2(pipeOutFd[1], STDOUT_FILENO, pipeOutFd[0]);
		try
		{
			_request.execCgiClass();
		}
		catch(const std::exception& e)
		{
			makeDup2(stdFd[FD_STDIN], STDIN_FILENO);
			makeDup2(stdFd[FD_STDOUT], STDOUT_FILENO);
			fd[FD_STDIN] = -2;
			fd[FD_STDOUT] = -2;
			throw std::runtime_error("Execve error");
		}
	}
	else
	{
		close(pipeInFd[0]);
		close(pipeOutFd[1]);
		_request.clearCgiClass();
		fd[FD_STDIN] = pipeInFd[1];
		fd[FD_STDOUT] = pipeOutFd[0];
	}
}

/* [!] : DELETE error need to be checked */
void Client::delete_request()
{
	std::string	target;
	struct stat	s_stat;

	target = _request.getRequestTarget();
	if (!target.size())
		throw ErrorExcept(400, "Bad request");
	if (target[target.size() - 1] == '/')
		throw ErrorExcept(403, "Forbidden");
	if (stat(target.c_str(), &s_stat) != 0)
		throw ErrorExcept(404, "Not found");
	if (S_ISREG(s_stat.st_mode))
	{
		if (std::remove(target.c_str()) != 0)
			throw ErrorExcept(404, "Not found");
	}
	else
		throw ErrorExcept(403, "Forbidden");
	_response.setRespCategory(SINGLE_RSP);
}

void Client::processRequest(std::string const & clientRequest, uint32_t & opt, int *fd, pid_t & childPid)
{
	stringMap	header;
	
	// std::cout << "REQUEST--->\n" << clientRequest << "\n<----" << _request.get_is_CRLF_header() << std::endl;
	if (!_request.get_is_CRLF_header())
	{
		_request.setPartRequest(clientRequest);
		if (_request.getPartRequest().find(CRLF_SEP) == std::string::npos)
		{
			opt = EPOLLIN;
			return;
		}
		_request.parseRequest();
		processRequestWithConfig();
	}
	else
	{
		_request.set_Body(clientRequest);
		_request.set_c_length_received(clientRequest.size());
		if (_request.getIsCgi() == true)
		{
			_cgiFD->setBufferWrite(_request.getBody());
			return;
		}
	}
	header = _request.getHeader();
	_request.displayRequest(); /* [Test only] */
	if (_request.getMethod() == "GET")
		get_request(opt, fd, childPid);
	else if (_request.getMethod() == "POST")
		post_request(header, opt, fd, childPid);
	else if (_request.getMethod() == "DELETE")
	{
		delete_request();
		setSingleResponse(204);
	}
}

vectLocationIt Client::findLongestMatch(std::string const &text, vectLocation &location)
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
		compare = (*it).getName();
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
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    struct tm tstruct;
    char buf[20];
    
    tstruct = *localtime(&tv.tv_sec);
    strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tstruct);
    std::string result(buf);
    char msBuf[4];
    snprintf(msBuf, sizeof(msBuf), "%03d", (int)(tv.tv_usec / 1000));
    return (result += msBuf);
}

bool Client::get_pathfile()
{
	size_t findFile = _request.get_Body().find("filename=\"");
	std::string body;

	body = _request.get_Body();
	while (this->_filepath.empty())
	{
		if (findFile != std::string::npos && _request.get_Body().find("filename=\"\"") == std::string::npos)
		{
			// std::cout << "\e[5m				SEARCH FILE POST PATH\e[0m" << std::endl;
			size_t endFile = _request.get_Body().find("\n", findFile + 10);
			std::string filename;

			std::copy(_request.get_Body().begin() + (findFile + 10), _request.get_Body().begin() + (endFile - 2), std::back_inserter(filename));
			endFile = _request.get_Body().find("\r\n\r\n", endFile);
			_request.delete_range_body_request(0, endFile + 4);
			if (filename.size())
			{
				this->_filepath = _request.getRequestTarget() + "/" + currentDateTimeString() + filename;
				// std::cout << "\n\n\n		PATH=["<< this->filepath <<"]\n\n\n" << std::endl;
				return (true);
			}
		}
		else
		{
			size_t end = _request.get_Body().find(this->_boundary_sep);
			if (end != std::string::npos)
				_request.delete_range_body_request(0, end + this->_boundary_sep.size() + 2);
			else
				_request.delete_range_body_request(0, _request.get_Body().size());
		}	
		findFile = _request.get_Body().find("filename=\"");
		if (findFile == std::string::npos)
			break ;
	}
	return (false);
}

void Client::get_boundary_from_header(stringMap &header)
{
	if (this->_boundary_sep.empty())
	{
		std::string	getboundary = header.find("Content-Type")->second;
		size_t pos = getboundary.find("boundary=");
		if (pos != std::string::npos)
		{
			getboundary.erase(0, pos + 9); 
			this->_boundary_sep = "--" + getboundary;
		}
		// std::cout << "		BOUNDARY=" << this->_boundary_sep << std::endl;
	}
}

void Client::write_in_file_post()
{
	std::string body;
	size_t		pos;
	
	if (this->_filepath.size() && _request.get_Body().size())
	{
		body = _request.get_Body();
		std::fstream file(this->_filepath.c_str(), std::ios::out | std::ios::binary | std::ios::app);
		if (file.is_open())
		{
			pos = body.find(this->_boundary_sep);
			if (pos != std::string::npos)
			{
				this->_filepath.clear();
				body = body.substr(0 , pos - 2);
				_request.delete_range_body_request(0, pos + this->_boundary_sep.size() + 2);
				std::cout << "		\033[43m\e[5mFILE WRITE DONE\033[0m\e[0m" << std::endl;
			}
			file << body;
			if (file.bad() || file.fail())
			{
				file.close();
				throw ErrorExcept(500, "Internal Server Error - Write in file failed");
			}
			else
				file.close();
		}
		else
			throw ErrorExcept(500, "Internal Server Error - Write in file failed");
	}
}

void Client::process_multipart_data()
{
	if (get_pathfile())
		std::cout << "\e[5mNEW FILE\e[0m->"<< this->_filepath << std::endl;
	if (this->_filepath.size())
	{
		while (1)
		{
			if (this->_filepath.empty())
				break ;
			write_in_file_post();
			if (get_pathfile())
				continue ;
			break;
		}
	}
}

void Client::set_last_time_activity(){this->_last_time = time(NULL);}

void Client::set_time_activity(){this->_time_now = time(NULL);}

bool Client::is_client_timeout()
{
	set_time_activity();
	if (difftime(this->_time_now, this->_last_time) > 62)
	{
		// std::cout << "			CLIENT TIME OUT REACHED!!!!!!!!!!!!!!" << std::endl; /* [Test only] */
		throw ErrorExcept(408, "Client Timeout"); /* [?] : When to send 408 response to client? On long upload only? */
	}
    return false;
}
