/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:41 by barandri          #+#    #+#             */
/*   Updated: 2025/10/23 15:59:35 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::string	Response::_spareDefErrPage;

Response::Response() : _respCategory(STD_RSP), _pos(0){}

Response::Response(int const & statusCode) : _statusCode(statusCode), _pos(0){}

Response::Response(int const & statusCode, std::string const & type) : _statusCode(statusCode), _type(type), _pos(0){}

Response::Response(Response const & other) :
	_statusCode(other._statusCode),
	_type(other._type),
	_body(other._body),
	_statusLine(other._statusLine),
	_header(other._header),
	_fullResponse(other._fullResponse),
	_respCategory(other._respCategory),
	_respStat(other._respStat),
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
		_originPath = other._originPath;
		_originBody = other._originBody;
		_pos = other._pos;
	}
	return *this;
}

Response::~Response(){}

int Response::getStatusCode() const
{
    return _statusCode;
}

void Response::setType(std::string const &type)
{
	_type = type;
}

std::string Response::getType() const
{
    return _type;
}

void Response::setStatusCode(int const &statusCode)
{
	_statusCode = statusCode;
}

void Response::setRespCategory(int const &category)
{
	_respCategory = category;
}

size_t Response::getPosition() const
{
    return _pos;
}

void Response::setPosition(size_t const &newPos)
{
	_pos = newPos;
}

int Response::getRespStat() const
{
    return _respStat;
}

void Response::setRespStat(int const &stat)
{
	_respStat = stat;
}

std::string Response::getOriginPath() const
{
    return _originPath;
}

void Response::setOriginPath(std::string const &path)
{
	_originPath = path;
}

void Response::addHeader(std::string const &header, std::string const &content)
{
	_header[header] = content;
}

int Response::getRespCategory() const
{
    return _respCategory;
}

std::string Response::getFullResponse()
{
	std::string	headerString;
	std::string	endLine = "\r\n";

	headerString = convertStringMapToString(_header, endLine);
	_fullResponse = _statusLine + endLine + headerString + endLine + _body + endLine;
	return _fullResponse;
}

std::string Response::getBody() const
{
    return _body;
}

std::string Response::getOriginBody() const
{
    return _originBody;
}

void Response::setOriginBody(std::string const &body)
{
	_originBody = body;
}

void	Response::clearResponse()
{
	_fullResponse.clear();
}

void Response::addStatusCode(std::string & content)
{
	std::string	replace;

	replace = convertIntToString(_statusCode);
	replace += " : " + getReasonPhrase(_statusCode);
	replaceString(content, replace, "$status");
}

void Response::buildErrorPage(int const & status, t_errPage & errPage, std::string & filePath, int & fdType)
{
	stringVectIt	it;
	std::string		content;

	filePath = ERR_PATH;
	fdType = STD_FILE;
	if (errPage.listError.size())
	{
		it = std::find(errPage.listError.begin(), errPage.listError.end(), convertIntToString(status));
		if (it != errPage.listError.end())
			filePath = errPage.filePath;
	}
	try
	{
		content = readFileToString(filePath);
		if (filePath == ERR_PATH)
			addStatusCode(content);
		_originPath = filePath;
	}
	catch(const std::exception& e)
	{
		std::cout << "Error on error page" << std::endl;
		content = _spareDefErrPage;
		addStatusCode(content);
		_originPath = "sparedefaultpage.html";
	}
	if (content.size() <= _BUFFER)
	{
		_body = content;
		_respStat = OP_ONCE;
		_type = Mime::getFileType(_originPath);
		generateStandardResponse(content);
	}
	else
	{
		_originBody = content;
		_respStat = OP_START;
	}
}

void Response::setStatusLine()
{
	_statusLine = "HTTP/1.1 " + convertIntToString(_statusCode) + " " + getReasonPhrase(_statusCode);
}

void	Response::generateStandardResponse(std::string const &  body)
{
	setStatusLine();
	_body = body;
	_header.clear();
	_header["Content-Length"] = convertIntToString(_body.size() + 2);
	_header["Content-Type"] = _type;
	_header["Connection"] = "keep-alive";
}

void Response::generateChunkedResponse(std::string const & body)
{
	setStatusLine();
	_body = body;
	chunkString(_body, _respStat);
	_header.clear();
	_header["Connection"] = "keep-alive";
	_header["Content-Type"] = _type;
	_header["Transfer-Encoding"] = "chunked";
}

void Response::generateCgiChunkedResponse(std::string const & body, stringMap const & header, int const & status)
{
	setStatusLine();
	_body = body;
	chunkString(_body, status);
	if (header.size())
	{
		_header.clear();
		_header["Connection"] = "keep-alive";
		_header["Content-Type"] = _type;
		_header["Transfer-Encoding"] = "chunked";
	}
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
		}
		else if (e._type == REDIR_RSP)
		{
			RedirExcept & redir = dynamic_cast<RedirExcept & >(e);
			generateRedirectResponse(redir);
			_respStat = OP_ONCE;
		}
	}
	catch(const std::exception& e)
	{
		/*
			What to do when an error occurs when errorPage file is corrupted? Stop the program? [barandri]
		*/
		throw std::runtime_error("AExcept error");
	}
	_respCategory = e._type;
}

void	Response::generateErrorResponse(ErrorExcept & e, t_errPage & errPage, std::string & filePath, int & fdType)
{
	_statusCode = e._statusCode;
	setStatusLine();
	buildErrorPage(_statusCode, errPage, filePath, fdType);
	_type = Mime::getFileType(_originPath);
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
	std::string	list = convertListing(d._dirList);

	_statusCode = d._statusCode;
	_type = "text/html";
	setStatusLine();
	_header.clear();
	_body = LDirExcept::_part[0] + list + LDirExcept::_part[1];
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

void Response::initSpareDefErrPage()
{
	_spareDefErrPage += "<!DOCTYPE html><html><head>"
							"<meta charset=\"UTF-8\">"
							"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
							"<title>Default error page</title>"
							"<style>body {min-height: 100vh;margin: 0;font-family: sans-serif;background-color: white;color: black;display: flex;justify-content: center;align-items: center;}"
							".content {padding: 40px;background-color: #f0f0f0;border: 1px solid black;max-width: 90%;text-align: center;}"
							"h1 {margin-bottom: 10px;}</style>"
							"</head><body><div class=\"content\"><h1>ERROR</h1><h2>$status</h2></div></body></html>";
}

std::string Response::getSpareDefErrPage()
{
    return _spareDefErrPage;
}

std::string Response::getReasonPhrase(int const &statusCode)
{
	std::string result;

    switch (statusCode)
	{
		case 100:
			result = "Continue";
		break;
		case 101:
			result = "Switching protocols";
		break;
		case 200:
			result = "OK";
		break;
		case 201:
			result = "Created";
		break;
		case 202:
			result = "Accepted";
		break;
		case 203:
			result = "Non-Authoritative Information";
		break;
		case 204:
			result = "No Content";
		break;
		case 205:
			result = "Reset Content";
		break;
		case 206:
			result = "Partial Content";
		break;
		case 300:
			result = "Multiple Choices";
		break;
		case 301:
			result = "Moved Permanently";
		break;
		case 302:
			result = "Found";
		break;
		case 303:
			result = "See Other";
		break;	
		case 304:
			result = "Not Modified";
		break;
		case 305:
			result = "Use Proxy";
		break;
		case 307:
			result = "Temporary Redirect";
		break;
		case 308:
			result = "Permanent Redirect";
		break;
		case 400:
			result = "Bad Request";
		break;
		case 401:
			result = "Unauthorized";
		break;
		case 402:
			result = "Payment Required";
		break;
		case 403:
			result = "Forbidden";
		break;
		case 404:
			result = "Not Found";
		break;
		case 405:
			result = "Method Not Allowed";
		break;
		case 406:
			result = "Not Acceptable";
		break;
		case 407:
			result = "Proxy Authentication required";
		break;
		case 408:
			result = "Request Timeout";
		break;
		case 409:
			result = "Conflict";
		break;
		case 410:
			result = "Gone";
		break;
		case 411:
			result = "Length Required";
			break;
		case 412:
			result = "Precondition Failed";
		break;
		case 413:
			result = "URI Too Long";
		break;
		case 414:
			result = "Content Too Large";
		break;
		case 415:
			result = "Unsupported Media Type";
		break;
		case 416:
			result = "Range Not Satisfiable";
		break;
		case 417:
			result = "Expectation Failed";
		break;
		case 421:
			result = "Misdirected Request";
		break;
		case 422:
			result = "Unprocessable Content";
		break;
		case 426:
			result = "Upgrade required";
		break;
		case 500:
			result = "Internal Server Error";
		break;
		case 501:
			result = "Not Implemented";
		break;
		case 502:
			result = "Bad Gateway";
		break;
		case 503:
			result = "Service Unavailable";
		break;
		case 504:
			result = "Gateway Timeout";
		break;
		case 505:
			result = "HTTP Version Not Supported";
		break;
		default:
			throw std::runtime_error("NO STATUS CODE");
		break;
	}
	return result;
}