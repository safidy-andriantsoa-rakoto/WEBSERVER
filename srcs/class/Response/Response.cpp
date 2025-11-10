/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:41 by barandri          #+#    #+#             */
/*   Updated: 2025/11/10 20:17:26 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() : _respCategory(STD_RSP), _regFileFd(-1), _contentLength(0), _pos(0){}

Response::Response(int const & statusCode) : _statusCode(statusCode), _regFileFd(-1), _contentLength(0), _pos(0){}

Response::Response(int const & statusCode, std::string const & type) : _statusCode(statusCode), _type(type), _regFileFd(-1), _contentLength(0), _pos(0){}

Response::Response(Response const & other) :
	_statusCode(other._statusCode),
	_type(other._type),
	_body(other._body),
	_statusLine(other._statusLine),
	_header(other._header),
	_fullResponse(other._fullResponse),
	_respCategory(other._respCategory),
	_respStat(other._respStat),
	_regFileFd(other._regFileFd),
	_contentLength(other._contentLength),
	_originPath(other._originPath),
	_originBody(other._originBody),
	_pos(other._pos){}

Response &Response::operator=(Response const & other)
{
	if (this != &other)
	{
		_statusCode = other._statusCode;
		_type = other._type;
		_body = other._body;
		_statusLine = other._statusLine;
		_header = other._header;
		_fullResponse = other._fullResponse;
		_respCategory = other._respCategory;
		_respStat = other._respStat;
		_regFileFd = other._regFileFd;
		_contentLength = other._contentLength;
		_originPath = other._originPath;
		_originBody = other._originBody;
		_pos = other._pos;
	}
	return *this;
}

Response::~Response(){}

int Response::getStatusCode() const{return _statusCode;}

void Response::setType(std::string const &type){_type = type;}

std::string Response::getType() const{return _type;}

void Response::setStatusCode(int const &statusCode){_statusCode = statusCode;}

void Response::setRespCategory(int const &category){_respCategory = category;}

size_t Response::getPosition() const{return _pos;}

void Response::setPosition(size_t const &newPos){_pos = newPos;}

int & Response::getRegFileFd(){return _regFileFd;}

void Response::setRegFileFd(int const &fd){ _regFileFd = fd;}

void Response::closeRegFileFd()
{
	if (_regFileFd != -1)
		close(_regFileFd);
}

size_t Response::getContentLength() const{return _contentLength;}

void Response::setContentLength(size_t const &size){_contentLength = size;}

int Response::getRespStat() const{return _respStat;}

void Response::setRespStat(int const &stat){_respStat = stat;}

std::string Response::getOriginPath() const{return _originPath;}

void Response::setOriginPath(std::string const &path){_originPath = path;}

void Response::addHeader(std::string const &header, std::string const &content){_header[header] = content;}

int Response::getRespCategory() const{return _respCategory;}

std::string Response::getFullResponse()
{
	std::string	headerString;
	std::string	endLine = "\r\n";

	headerString = convertStringMapToString(_header, endLine);
	_fullResponse = _statusLine + endLine + headerString + endLine + _body;
	return _fullResponse;
}

std::string Response::getBody() const{return _body;}

std::string Response::getOriginBody() const{return _originBody;}

void Response::setOriginBody(std::string const &body){_originBody = body;}

void	Response::clearResponse(){_fullResponse.clear();}

void Response::addStatusCode(std::string & content)
{
	std::string	replace;

	replace = convertIntToString(_statusCode);
	replace += " : " + getReasonPhrase(_statusCode);
	if (_statusCode == 505)
		replace += "\n" + _body; //NEED TO TEST FOR ERROR 505
	replaceString(content, replace, "$status");
}

void Response::setupDefaultStringPage(std::string const &content)
{
	if (content.size() <= _BUFFER)
	{
		_body = content;
		_respStat = OP_ONCE;
		generateStringSingleResponse(content);
	}
	else
	{
		_originBody = content;
		_respStat = OP_START;
	}
}

void Response::setupDefaultErrorPage()
{
	std::string	content;

	content = readFileToString(ERR_PATH);
	addStatusCode(content);
	setupDefaultStringPage(content);
	_respCategory = ERR_RSP;
}

void Response::setupCustomErrorPage(std::string const & filePath)
{
	_regFileFd = openRegFile(filePath);
	_contentLength = getFileSize(filePath);
	if (_contentLength <= _BUFFER)
	{
		_respStat = OP_ONCE;
		generateRegStandardResponse();
	}
	else
		_respStat = OP_START;
	_respCategory = STD_RSP;
}

void Response::buildErrorPage(int const & status, t_errPage & errPage, std::string & filePath, int & fdType)
{
	stringVectIt	it;

	filePath = ERR_PATH;
	fdType = STD_FILE;
	if (errPage.listError.size())
	{
		if (std::count(errPage.listError.begin(), errPage.listError.end(), convertIntToString(status)) == 1)
			filePath = errPage.filePath;
	}
	try
	{
		_originPath = filePath;
		if (filePath == ERR_PATH)
			setupDefaultErrorPage();
		else
			setupCustomErrorPage(filePath);
	}
	catch(const std::exception& e)
	{
		std::string content = BACKUP_ERR_PAGE;
		addStatusCode(content);
		_originPath = "backup.html";
		setupDefaultStringPage(content);
	}
	_type = Mime::getFileType(_originPath);
}

void Response::setStatusLine()
{
	_statusLine = "HTTP/1.1 " + convertIntToString(_statusCode) + " " + getReasonPhrase(_statusCode);
}

void	Response::generateStringSingleResponse(std::string const &  body)
{
	setStatusLine();
	_body = body;
	_header.clear();
	_header["Content-Length"] = convertIntToString(_body.size());
	_header["Content-Type"] = _type;
	_header["Connection"] = "keep-alive";
}

void Response::generateRegStandardResponse()
{
	setStatusLine();
	_body = readFromRegFileFd(_regFileFd, _respStat);
	_header.clear();
	_header["Content-Length"] = convertIntToString(_body.size());
	_header["Content-Type"] = _type;
	_header["Connection"] = "keep-alive";
}

void Response::generateRegChunkedResponse()
{
	setStatusLine();
	_body = readFromRegFileFd(_regFileFd, _respStat);
	_header.clear();
	_header["Content-Length"] = convertIntToString(_contentLength);
	_header["Content-Type"] = _type;
	_header["Connection"] = "keep-alive";
}

void Response::generateStringChunkedResponse(std::string const & body)
{
	setStatusLine();
	_body = body;
	_header.clear();
	_header["Connection"] = "keep-alive";
	_header["Content-Type"] = _type;
	_header["Content-Length"] = convertIntToString(_originBody.size());
}

void Response::generateCatchedResponse(AExcept & e, t_errPage & errPage, std::string & filePath, int & fdType)
{
	try
	{
		if (e._type == ERR_RSP)
		{
			ErrorExcept & err = dynamic_cast<ErrorExcept & >(e);
			generateErrorResponse(err, errPage, filePath, fdType);
		}
		else if (e._type == LDIR_RSP)
		{
			LDirExcept & dir = dynamic_cast<LDirExcept & >(e);
			generateLDirResponse(dir);
			_respStat = OP_ONCE;
			_respCategory = e._type;
		}
		else if (e._type == REDIR_RSP)
		{
			RedirExcept & redir = dynamic_cast<RedirExcept & >(e);
			generateRedirectResponse(redir);
			_respStat = OP_ONCE;
			_respCategory = e._type;
		}
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error("AExcept error");
	}
}

void	Response::generateErrorResponse(ErrorExcept & e, t_errPage & errPage, std::string & filePath, int & fdType)
{
	_statusCode = e._statusCode;
	setStatusLine();
	buildErrorPage(_statusCode, errPage, filePath, fdType);
}

void Response::generateRedirectResponse(RedirExcept & r)
{
	_statusCode = r._statusCode;
	_type = "text/html";
	setStatusLine();
	_header.clear();
	_body.clear();
	_header["Location"] = r._uri;
	_header["Connection"] = "keep-alive";
	_header["Content-Length"] = "0";
}

void Response::generateLDirResponse(LDirExcept &d)
{
	std::string	list = createListingDirectoryLink(d._dirList);

	_statusCode = d._statusCode;
	_type = "text/html";
	setStatusLine();
	_header.clear();
	_body = HEAD_LDIR + list + TAIL_LDIR;
	_header["Content-Length"] = convertIntToString(_body.size());
	_header["Connection"] = "keep-alive";
}

void Response::generateSingleResponse(int const &statusCode)
{
	_statusCode = statusCode;
	_respCategory = SINGLE_RSP;
	_respStat = OP_ONCE;
	_type = "text/html";
	setStatusLine();
	_header.clear();
	_body.clear();
	_header["Connection"] = "keep-alive";
	_header["Content-Length"] = "0";
}

/* [!] : Review unused status code and do update on list too */
std::string Response::getReasonPhrase(int const &statusCode)
{
	std::string result;

    switch (statusCode)
	{
		case 200: result = "OK"; break;
		case 201: result = "Created"; break;
		case 204: result = "No Content"; break;
		case 301: result = "Moved Permanently"; break;
		case 302: result = "Found"; break;	
		case 307: result = "Temporary Redirect"; break;
		case 308: result = "Permanent Redirect"; break;
		case 400: result = "Bad Request"; break;
		case 403: result = "Forbidden"; break;
		case 404: result = "Not Found"; break;
		case 405: result = "Method Not Allowed"; break;
		case 408: result = "Request Timeout"; break;
		case 411: result = "Length Required"; break;
		case 413: result = "Content Too Large"; break;
		case 414: result = "URI Too Long"; break;
		case 415: result = "Unsupported Media Type"; break;
		case 421: result = "Misdirected Request"; break;
		case 500: result = "Internal Server Error"; break;
		case 501: result = "Not Implemented"; break;
		case 502: result = "Bad Gateway"; break;
		case 503: result = "Service Unavailable"; break;
		case 504: result = "Gateway Timeout"; break;
		case 505: result = "HTTP Version Not Supported"; break;
		default: throw std::runtime_error("Status code not supported"); break;
	}
	return result;
}