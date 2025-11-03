/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendingResponse.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:13:07 by barandri          #+#    #+#             */
/*   Updated: 2025/11/03 15:06:50 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

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
		rspString = (*it).getBodyResponse();
	else
		rspString = (*it).getFullResponse();
	std::cout << "		Sending file from " << (*it).getRspOriginPath() << std::endl;
	std::cout << rspString << std::endl;
	send_value(fdCli, rspString);
	if ((*it).getResponseStat() == OP_START)
		(*it).setResponseStat(OP_ONGOING);
	if ((*it).getResponseStat() == OP_ONCE || (*it).getResponseStat() == OP_CLOSE)
	{
		std::cout << "			Ending send response" << std::endl;
		epoll.epoll_update_event(fdCli, EPOLLIN);
		(*it).getObjRequest().set_is_CRLF_header(false);
	}
}

void send_value(int const & fd, std::string & _send_content)
{
    size_t to_send = _send_content.size();

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
        throw ErrorExcept(400, "Connection closed by peer"); //400?
    _send_content.clear();
}
