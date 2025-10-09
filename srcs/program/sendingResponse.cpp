/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendingResponse.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:13:07 by barandri          #+#    #+#             */
/*   Updated: 2025/10/09 07:36:24 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/*
	See other cases of response, for successfull response
*/
void	sendRedirOrLdirResponse(VClientIt & it, int & fdCli, EpollCreation & epoll)
{
	std::string	responseString;

	responseString = (*it).getFullResponse();
	// std::cout <<"\033[35mSending redirectory/listing directory response\n" << responseString << "\033[0m" << std::endl;
	send_value(fdCli, responseString);
	epoll.epoll_update_event(fdCli, EPOLLIN);
}

void	sendHtmlFileResponse(VClientIt & it, int & fdCli, EpollCreation & epoll)
{
	std::string	responseString;
	int			fileStatus;

	fileStatus = (*it).getFileStatus();
	if (fileStatus == OP_ONCE)
	{
		std::cout << "\033[35mSending standard response" << "\033[0m"<< std::endl;
		(*it).setStandardResponse();
	}
	else
	{
		std::cout << "\033[35mSending chunked response" << "\033[0m"<< std::endl;
		(*it).setChunkedResponse();
	}
	if (fileStatus == OP_ONGOING || fileStatus == OP_CLOSE)
		responseString = (*it).getBodyResponse() + "\r\n";
	else
		responseString = (*it).getFullResponse();
	std::cout <<"\e[45mResponse from the file : " << (*it).getPtrFileFd()->getFilePath() << "\e[0m" << std::endl;
	send_value(fdCli, responseString);
    (*it).getPtrFileFd()->clearBuffer();
	if (fileStatus != OP_ONGOING && fileStatus != OP_START)
		epoll.epoll_update_event(fdCli, EPOLLIN);
	if (fileStatus == OP_START)
		(*it).updateFileStatus(OP_ONGOING);
	(*it).increaseFileSent(_BUFFER);
	if (fileStatus == OP_ONCE || fileStatus == OP_CLOSE)
		(*it).clearPtrFileFd();
}

void	sendCgiResponse(VClientIt & it, int & fdCli, EpollCreation & epoll)
{
	std::string	responseString;
	int			cgiStatus;

	cgiStatus = (*it).getCgiFdStatus();
    if (cgiStatus == OP_ONGOING && (*it).getPtrCgiFd()->getBuffer().size() == 0)
    {
        // std::cout << "          Nothing inside the buffer yet !!!!!\n";
        return;
    }
    std::cout << "\033[35mSending cgi chunked response to " << fdCli << " with status = " << cgiStatus << "\033[0m"<< std::endl;
	(*it).setCgiChunkedResponse();
	if (cgiStatus == OP_ONGOING || cgiStatus == OP_CLOSE)
		responseString = (*it).getBodyResponse() + "\r\n";
	else
		responseString = (*it).getFullResponse();
	// std::cout <<"\e[45mCgi response sent : \n" << responseString << "\e[0m" << std::endl;
	send_value(fdCli, responseString);
    (*it).getPtrCgiFd()->clearBuffer();
	if (cgiStatus != OP_ONGOING && cgiStatus != OP_START)
        epoll.epoll_update_event(fdCli, EPOLLIN);
	if (cgiStatus == OP_START)
		(*it).updateCgiFdStatus(OP_ONGOING);
	if (cgiStatus == OP_CLOSE)
		(*it).clearPtrFileFd();
}

/*
    Need to throw exception if error on sending? because some connexion stopped from the client
    Other amelioration?
*/
void send_value(int const & _acceptStatus, std::string & _send_content)
{

    size_t to_send = _send_content.size();

    // std::cout << "Size to send = " << _send_content.size() << std::endl;
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
