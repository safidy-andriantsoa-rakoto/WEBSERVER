/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:50:14 by barandri          #+#    #+#             */
/*   Updated: 2025/07/28 08:36:25 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	getRequestBody(std::string const & requeststring, Request & request, size_t const & pos)
{
	int			length;
	std::string		content;
	stringMapIt	it;

	length = 0;
	it = request.getHeader().find("Content-Length");
	if (it == request.getHeader().end())
		return ;
	length = atoi((*it).second.c_str());
	content = requeststring.substr(pos, static_cast<size_t>(length));
	//Length error management to see
	request.setBody(content);
}

void parseRequest(std::string const & requeststring, Request & request)
{
	size_t		pos;
	size_t		startPos;
	std::string		sep = "\r\n";
	std::string		line;
	stringVect	bodyVect;

	startPos = 0;
	pos = requeststring.find(sep, startPos);
	if (pos == std::string::npos)
		throw std::runtime_error("No request line found");
	line = requeststring.substr(startPos, pos - startPos);
	request.setRequestLine(line);
	startPos = pos + sep.size();
	while (pos != std::string::npos)
	{
		pos = requeststring.find(sep, startPos);
		if (pos == startPos)
			break ;
		line = requeststring.substr(startPos, pos - startPos);
		bodyVect = strictSplit(line, ": ");
		request.addHeader(bodyVect[0], bodyVect[1]);
		startPos = pos + sep.size();
		bodyVect.clear();
	}
	getRequestBody(requeststring, request, pos + sep.size());
	request.displayRequest();
}