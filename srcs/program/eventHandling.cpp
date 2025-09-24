/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/09/24 15:46:22 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	handleServerEvent(Handler & handler, int const & i, EpollCreation & epoll)
{
	int 			fdc;
	vectServerIt	it = handler.getServer(i);

	fdc = (*it).accept_client_connexion();
	epoll.epoll_control(EPOLLIN, fdc);
	handler.addToVectClient(handler.getVectServer()[i], fdc);
	std::cout << "\033[42mNEW CLIENT JOIN = " << fdc << "--> SERVER=" << (*it).get_fdSocket() << "\033[0m" << std::endl;
}

void	manageRequestFromClient(VClientIt & it, int & fd, EpollCreation & epoll, Handler & handler)
{
	int			fdFile;
	uint32_t	fileFdOpt;
	std::string client_request;
	// std::string limit = "\r\n\r\n";
	std::string body;
	std::string filePath;
	Request		tmpRequest;
	// std::string limiter = "--";
	// std::string limiterEnd = "--";
		
	tmpRequest = (*it).getobjREquest();
	(*it).ClientRequest();
	client_request = (*it).getClientRequest();
	// std::cout << "-->> \033[042m ManageRequestFromClient : request_size = " << client_request.size() << "\033[0m" << std::endl;
	if (client_request.size() == 0)
	{
		(*it).clearFileFd();
		throw std::runtime_error("ManageRequestFromClient - request size = 0");
	}
	try
	{
		/*
			Adding FLAG [Safidy]
			Need to have the complete request before parsing/processing request
			Client stay at EPOLLIN
		*/
		fdFile = (*it).processRequest(client_request, fileFdOpt, filePath);
		// size_t pos = client_request.find(limit);
		// 	if (pos != std::string::npos)
		// 	{
		// 		std::string header = client_request.substr(0, pos);
		// 		std::cout << "HEADER====>" << header << "]" << std::endl;
		// 		body = client_request.substr(pos + limit.size(), client_request.size());
		// 		if ((*it).getBoundaryLimit().size() && body.size())
		// 		{
		// 			limiter += (*it).getBoundaryLimit() + "\r\n";
		// 			size_t is_boundary = body.find(limiter);
		// 			body.erase(0, is_boundary);
		// 			while (is_boundary != std::string::npos)
		// 			{
		// 				body.erase(0, is_boundary + limiter.size());
		// 				is_boundary = body.find(limiter);
		// 				if (is_boundary != std::string::npos)
		// 				{
		// 					std::string tmp = body.substr(0, is_boundary - 2);
		// 					stringVect separate = strictSplit(tmp, limit);
		// 					if (separate.size() == 2)
		// 					{
		// 						stringVect var = strictSplit(*(separate.begin()), ": ");
		// 						if (var.size())
		// 							(*it).getobjREquest().set_post_body(var, *(separate.begin() + 1));
		// 					}
		// 					std::cout << "BODY=====>[" << tmp << "]" << std::endl;
		// 				}
		// 				else
		// 				{
		// 					std::cout << "BODY==END===>[" << body << "]" << std::endl;
		// 					stringVect separate = strictSplit(body, limit);
		// 					if (separate.size() == 2)
		// 					{
		// 						stringVect var = strictSplit(*(separate.begin()), ": ");
		// 						if (var.size())
		// 							(*it).getobjREquest().set_post_body(var, *(separate.begin() + 1));
		// 					}
		// 				}
		// 			}
		// 		}
			// (*it).getobjREques().displayRequestMultipartData();
		
		// int fdCli = (*it).get_clientFD();
		// epoll.epoll_update_event(fdCli, EPOLLOUT);
			(*it).updateResponseCategory(STD_RSP);
		// }
	}
	catch(AExcept & e)
	{
		// std::cerr << "\033[031mProcess Request exception catched : " << e._errMsg << "\033[0m" << std::endl;
		(*it).setCatchedResponse(e, fdFile, filePath);
		fileFdOpt = EPOLLIN;
		if ((*it).getCategoryResponse() == REDIR_RSP || (*it).getCategoryResponse() == LDIR_RSP)
		{
			epoll.epoll_update_event(fd, EPOLLOUT);
			return;
		}
	}
	handler.addToVectFileFd(it, fdFile, fd, filePath);
	epoll.epoll_control(fileFdOpt, fdFile);
}

/*
	See other cases of response, for successfull response
*/
void	sendingResponseToClient(VClientIt & it, int & fd, EpollCreation & epoll)
{
	std::string	responseString;
	int			statusCode;
	int			fileStatus;
	int			responseCategory = (*it).getCategoryResponse();
	std::cout << "-->> \033[45mSendingResponseToClient : " << responseCategory << "\033[0m" << std::endl;

	if (responseCategory == REDIR_RSP || responseCategory == LDIR_RSP)
	{
		responseString = (*it).getFullResponse();
		std::cout <<"\033[35mSending redirectory/listing directory response" << "\033[0m" << std::endl;
		send_value(fd, responseString);
		epoll.epoll_update_event(fd, EPOLLIN);
		return ;
	}
	fileStatus = (*it).getFileStatus();
	if (responseCategory == STD_RSP)
		statusCode = 200;
	else
		statusCode = (*it).getStatusCodeResponse();
	if (fileStatus == OP_ONCE)
	{
		std::cout << "\033[35mSending standard response" << "\033[0m"<< std::endl;
		(*it).setStandardResponse(statusCode);
	}
	else
	{
		std::cout << "\033[35mSending chunked response" << "\033[0m"<< std::endl;
		(*it).setChunkedResponse(statusCode);
	}
	if (fileStatus == OP_ONGOING || fileStatus == OP_CLOSE)
		responseString = (*it).getBodyResponse() + "\r\n";
	else
		responseString = (*it).getFullResponse();
	// std::cout <<"\e[45mResponse: \n" << responseString << "\e[0m" << std::endl;
	std::cout <<"\e[45mResponse from the file : " << (*it).getFileFD()->getFilePath() << "\e[0m" << std::endl;
	send_value(fd, responseString);
	if (fileStatus != OP_ONGOING && fileStatus != OP_START)
		epoll.epoll_update_event(fd, EPOLLIN);
	if (fileStatus == OP_START)
		(*it).updateFileStatus(OP_ONGOING);
	(*it).increaseFileSent(_BUFFER);
	if (fileStatus == OP_ONCE)
		(*it).clearFileFd();
}

void	handleClientEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx)
{
	uint32_t	event = epoll.getEvent(eventIdx);
	VClientIt	it = handler.getClient(i);
	int			fd = (*it).get_clientFD();
	
	if ((event & EPOLLIN) && (*it).getConnexionStat() == true)
		manageRequestFromClient(it, fd, epoll, handler);
	else if ((event & EPOLLOUT) && (*it).getConnexionStat() == true)
		sendingResponseToClient(it, fd, epoll);
	else
		throw std::runtime_error("HandleClientEvent error");
}

void	readFromFileFd(vectFileFdIt & it, EpollCreation & epoll, Handler & handler, int const & i)
{
	int			fdClient;
	int			bytesRead;
	char		buffer[_BUFFER];
	std::string	bufferString;
	
	std::cout << "\033[43mFileFd EPOLLIN : " << (*it)->getClientFd() << " ; OP = " << (*it)->getStatus() << "\033[0m" << std::endl;
	(*it)->manageFd();
	if ((*it)->getStatus() == OP_START)
		return;
	bytesRead = read((*it)->getFileFd(), buffer, _BUFFER);
	if (bytesRead <= 0)
	{
		(*it)->setStatus(OP_CLOSE);
		(*it)->clearBuffer();
		closeFileEvent(handler, i, epoll);
		return ;
	}
	if ((*it)->getStatus() == OP_OPEN && (*it)->getFileTotalSize() <= static_cast<size_t>(bytesRead))
		(*it)->setStatus(OP_ONCE);
	bufferString.append(buffer, bytesRead);
	(*it)->setBuffer(bufferString);
	(*it)->restoreFd();
	if ((*it)->getStatus() == OP_ONGOING && ((*it)->getFileTotalSize() == (*it)->getFileSentSize() + static_cast<size_t>(bytesRead)))
		(*it)->setStatus(OP_CLOSE);
	if ((*it)->getStatus() == OP_ONCE || (*it)->getStatus() == OP_OPEN)
	{
		fdClient = (*it)->getClientFd();
		if ((*it)->getStatus() == OP_OPEN)
			(*it)->setStatus(OP_START);
		epoll.epoll_update_event(fdClient, EPOLLOUT);
	}
	if ((*it)->getStatus() == OP_ONCE || (*it)->getStatus() == OP_CLOSE)
		closeFileEvent(handler, i, epoll);
}

void	handleFileEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx)
{
	vectFileFdIt	it = handler.getFileFd(i);
	uint32_t		event = epoll.getEvent(eventIdx);

	if (event & EPOLLIN)
		readFromFileFd(it, epoll, handler, i);
	else if (event & EPOLLOUT)
	{
		std::cout << "ATY		\033[43mFD EPOLLOUT FOR FILE IO" << "\033[0m" << std::endl;
	}
}
