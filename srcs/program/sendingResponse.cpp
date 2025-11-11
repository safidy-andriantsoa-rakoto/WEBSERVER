/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendingResponse.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:13:07 by barandri          #+#    #+#             */
/*   Updated: 2025/11/11 19:58:00 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	processCgiSetup(VClientIt & it)
{
	if ((*it).getPtrCgiFd() == NULL)
		return 0;
	bool respMode = (*it).getPtrCgiFd()->getRespMode();
	int	cgiReadStat = (*it).getPtrCgiFd()->getReadStat();
	if (respMode == false)
	{
		if (cgiReadStat != OP_CLOSE)
			return 1;
		(*it).setupCgiResponse();
	}
	return 0;
}

void	sendingResponseToClient(VClientIt & it, int & fdCli, EpollCreation & epoll)
{
	std::string	rspString;
	int			rspCategory = (*it).getRspCategory();
	std::cout << "-->> \033[45mSendingResponseToClient : " << (*it).getRspCategory() << "\033[0m" << std::endl;

	if (processCgiSetup(it) == 1)
		return;
	if ((*it).getResponseStat() != OP_ONCE)
	{
		if (rspCategory == CGI_RSP || rspCategory == ERR_RSP)
			(*it).setStringChunkedResponse();
		else
			(*it).setRegFdChunkedResponse();
	}
	if ((*it).getResponseStat() == OP_ONGOING || (*it).getResponseStat() == OP_CLOSE)
		rspString = (*it).getBodyResponse();
	else
		rspString = (*it).getFullResponse();
	std::cout << "		Sending file from " << (*it).getRspOriginPath() << std::endl;
	// std::cout << rspString << std::endl;
	send_value(fdCli, rspString);
	if ((*it).getResponseStat() == OP_START)
		(*it).setResponseStat(OP_ONGOING);
	if ((*it).getResponseStat() == OP_ONCE || (*it).getResponseStat() == OP_CLOSE)
	{
		/* [!] : Need to test this functionnality 408*/
		if ((*it).getObjResponse().getStatusCode() == 408)
			throw std::runtime_error("Close this client for timeout");
		epoll.epoll_update_event(fdCli, EPOLLIN);
		(*it).getObjRequest().set_is_CRLF_header(false);
		(*it).clearPtrCgiFd();
	}
}

void send_value(int const & fd, std::string & _send_content)
{
    size_t to_send = _send_content.size();
	ssize_t sent;

	if (to_send == 0)
		return;
    const char *resp =  _send_content.c_str();
    sent = send(fd, resp, to_send, 0);
    if (sent < 0)
        throw ErrorExcept(500, "Send_value error");
    if (sent == 0)
	{
        throw ErrorExcept(400, "Connection closed by peer"); /* [?] : error 400 ? */
	}
	_send_content.clear();
}
