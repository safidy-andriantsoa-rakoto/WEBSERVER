/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendingResponse.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:13:07 by barandri          #+#    #+#             */
/*   Updated: 2025/10/17 14:09:53 by barandri         ###   ########.fr       */
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
		epoll.epoll_update_event(fdCli, EPOLLIN);
	// std::cout << "		Rsp Stat = " << (*it).getResponseStat() << std::endl;
}

void	sendCgiResponse(VClientIt & it, int & fdCli, EpollCreation & epoll)
{
	std::string	rspString;
	int			cgiStatus;

	cgiStatus = (*it).getCgiFdStatus();
    if (cgiStatus == OP_ONGOING && (*it).getPtrCgiFd()->getBuffer().size() == 0)
    {
        std::cout << "          Nothing inside the buffer yet !!!!!\n";
        return;
    }
    std::cout << "\033[35mSending cgi chunked response to " << fdCli << " with status = " << cgiStatus << "\033[0m"<< std::endl;
	(*it).setCgiChunkedResponse();
	if (cgiStatus == OP_ONGOING || cgiStatus == OP_CLOSE)
		rspString = (*it).getBodyResponse() + "\r\n";
	else
		rspString = (*it).getFullResponse();
	std::cout <<"\e[45mCgi response sent : \n" << rspString.size() << "\e[0m" << std::endl;
	send_value(fdCli, rspString);
    (*it).getPtrCgiFd()->clearBuffer();
	if (cgiStatus != OP_ONGOING && cgiStatus != OP_START)
        epoll.epoll_update_event(fdCli, EPOLLIN);
	if (cgiStatus == OP_START)
		(*it).updateCgiFdStatus(OP_ONGOING);
}

/*
    Need to throw exception if error on sending? because some connexion stopped from the client
    Other amelioration?
*/
void send_value(int const & _acceptStatus, std::string & _send_content)
{

    size_t to_send = _send_content.size();

    // std::cout << "Size to send = " << _send_content.size() << std::endl;
    // std::cout << "Sent = " << _send_content << std::endl;
    if (_acceptStatus < 0)
        throw std::runtime_error("Invalid fd to send");
    const char *resp =  _send_content.c_str();
    ssize_t sent = send(_acceptStatus, resp, to_send, 0);
    if (sent < 0)
    {
        perror("Send failed");
        throw std::runtime_error("Send_value error");
    }
    else if (sent == 0)
        throw std::runtime_error("Connection closed by peer");
    _send_content.clear();
}
