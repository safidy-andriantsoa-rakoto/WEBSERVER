/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendingResponse.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:13:07 by barandri          #+#    #+#             */
/*   Updated: 2025/10/24 14:15:06 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/*
	See other cases of response, for successfull response
*/
void	sendSingleResponse(VClientIt & it, int & fdCli, EpollCreation & epoll)
{
	std::string	rspString;

	std::cout <<"\033[35mSending single response\n" << rspString.size() << "\033[0m" << std::endl;
	rspString = (*it).getFullResponse();
	send_value(fdCli, rspString);
	epoll.epoll_update_event(fdCli, EPOLLIN);
}

void	sendStandardResponse(VClientIt & it, int & fdCli, EpollCreation & epoll)
{
	std::string	rspString;

	if ((*it).getResponseStat() != OP_ONCE)
	{
		std::cout << "\033[35mSending chunked response" << "\033[0m"<< std::endl;
		(*it).setChunkedResponse();
	}
	if ((*it).getResponseStat() == OP_ONGOING || (*it).getResponseStat() == OP_CLOSE)
		rspString = (*it).getBodyResponse() + "\r\n";
	else
		rspString = (*it).getFullResponse();
	std::cout << "		Sending file from " << (*it).getRspOriginPath() << std::endl;
	// std::cout << rspString << std::endl;
	send_value(fdCli, rspString);
	if ((*it).getResponseStat() == OP_START)
		(*it).setResponseStat(OP_ONGOING);
	if ((*it).getResponseStat() == OP_ONCE || (*it).getResponseStat() == OP_CLOSE)
	{
		epoll.epoll_update_event(fdCli, EPOLLIN);
		(*it).getObjRequest().set_is_CRLF_header(false);
	}
		// std::cout << "		Rsp Stat = " << (*it).getResponseStat() << std::endl;
}

void	sendCgiResponse(VClientIt & it, int & fdCli, EpollCreation & epoll)
{
	std::string	rspString;
	int			cgiStatus;

	cgiStatus = (*it).getCgiFdStatus();
	if (cgiStatus == OP_OPEN)
		return;
    if (cgiStatus == OP_ONGOING && (*it).getPtrCgiFd()->getBuffer().size() == 0)
    {
        std::cout << "          Nothing inside the buffer yet !!!!!\n";
        return;
    }
    std::cout << "\033[35mSending cgi chunked response to " << fdCli << " with status = " << cgiStatus << "\033[0m"<< std::endl;
	if (cgiStatus == OP_START)
	{
		std::string	type = (*it).getPtrCgiFd()->getHeader()["Content-type"];
		(*it).getObjResponse().setType(type);
	}
	(*it).setCgiChunkedResponse();
	if (cgiStatus == OP_ONGOING || cgiStatus == OP_CLOSE)
		rspString = (*it).getBodyResponse() + "\r\n";
	else
		rspString = (*it).getFullResponse();
	// std::cout <<"\e[45mCgi response sent : \n" << rspString << "\e[0m" << std::endl;
	send_value(fdCli, rspString);
    (*it).getPtrCgiFd()->clearBufferRead();
	if (cgiStatus != OP_ONGOING && cgiStatus != OP_START)
	{
        epoll.epoll_update_event(fdCli, EPOLLIN);
		(*it).getObjRequest().set_is_CRLF_header(false);
	}
	if (cgiStatus == OP_START)
		(*it).updateCgiFdStatus(OP_ONGOING);
}

/*
    Review Error status Code !!!
*/
void send_value(int const & fd, std::string & _send_content)
{

    size_t to_send = _send_content.size();

    // std::cout << "Size to send = " << _send_content.size() << std::endl;
    // std::cout << "Sent = " << _send_content << std::endl;
    if (fd < 0)
        throw ErrorExcept(500, "Invalid fd to send");
    const char *resp =  _send_content.c_str();
    ssize_t sent = send(fd, resp, to_send, 0);
    if (sent < 0)
    {
        perror("Send failed");
        throw ErrorExcept(500, "Send_value error");
    }
    else if (sent == 0)
        throw ErrorExcept(400, "Connection closed by peer");
    _send_content.clear();
}
