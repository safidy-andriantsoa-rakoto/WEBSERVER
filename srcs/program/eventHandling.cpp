/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/10/13 08:10:22 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	handleServerEvent(Handler & handler, int const & i, EpollCreation & epoll)
{
	int 			fdc;
	vectServerIt	it = handler.getServerIterator(i);

	fdc = (*it).accept_client_connexion();
	epoll.epoll_control(EPOLLIN, fdc);
	handler.addToVectClient(handler.getVectServer()[i], fdc);
	std::cout << "\033[42mNEW CLIENT JOIN = " << fdc << "--> SERVER=" << (*it).get_fdSocket() << "\033[0m" << std::endl;
}

void	manageRequestFromClient(VClientIt & it, int & fdCli, EpollCreation & epoll, Handler & handler)
{
	uint32_t	epollOpt;
	std::string client_request;
	std::string limit = "\r\n\r\n";
	std::string body;
	// Request		tmpRequest;
	std::string limiter = "--";
	std::string limiterEnd = "--";
	int			fdFromRequest[2] = {-1, -1};
	
	// if ((*it).getObjRequest().getMethod() == "POST" && (*it).getPtrFileFd() && (*it).getPtrFileFd()->get_endFilePost())
	// 	(*it).clearPtrFileFd();
	// tmpRequest = (*it).getObjRequest();
	(*it).receiveClientRequest();
	client_request = (*it).getClientRequest();
	// std::cout << "-->> \033[042m ManageRequestFromClient : request_size = " << client_request.size() << "\033[0m" << std::endl;
	if (client_request.size() == 0)
	{
		(*it).clearPtrFileFd();
		(*it).clearPtrCgiFd();
		throw std::runtime_error("ManageRequestFromClient - request size = 0");
	}
	try
	{
		(*it).processRequest(client_request, epollOpt, fdFromRequest);
		// size_t pos = client_request.find(limit);
		// if (pos != std::string::npos)
		// {
		// 	std::string header = client_request.substr(0, pos);
		// 	std::cout << "HEADER====>" << header << "]" << std::endl;
		// 	body = client_request.substr(pos + limit.size(), client_request.size());
		// 	if ((*it).getBoundaryLimit().size() && body.size())
		// 	{
		// 		limiter += (*it).getBoundaryLimit() + "\r\n";
		// 		limiterEnd += (*it).getBoundaryLimit() + "--\r\n";
		// 		size_t is_boundary = body.find(limiter);
		// 		size_t is_boundaryEnd;
		// 		body.erase(0, is_boundary);
		// 		while (is_boundary != std::string::npos)
		// 		{
		// 			body.erase(0, is_boundary + limiter.size());
		// 			is_boundary = body.find(limiter);
		// 			is_boundaryEnd = body.find(limiterEnd);

		// 			if (is_boundary != std::string::npos)
		// 			{
		// 				// std::string tmp = body.substr(0, is_boundary - 2);
		// 				// stringVect separate = strictSplit(tmp, limit);
		// 				// if (separate.size())
		// 				// 	std::cout << "["<< separate[0] << "][" << separate[1] << "]" << std::endl;
		// 				stringVect separate;
		// 				std::string tmp = body.substr(0, is_boundary - 2);
		// 			 	size_t is_end = tmp.find("\r\n\r\n");
		// 				if (is_end != std::string::npos)
		// 				{
		// 					std::cout << "			aty eeeee" << std::endl;
		// 					if (is_end + 4 == tmp.size())
		// 					{
		// 						tmp.erase(is_end);
		// 						separate.push_back(tmp);
		// 						separate.push_back("");
		// 					}
		// 				}
		// 				else
		// 					separate = strictSplit(tmp, "\r\n\r\n");
		// 				if (separate.size() == 2)
		// 				{
		// 					stringVect var_line;
		// 					stringVect var;

		// 					size_t find_rn = separate[0].find("\r\n");
		// 					if (find_rn != std::string::npos)
		// 					{
		// 						while (find_rn != std::string::npos)
		// 						{
		// 							stringVect tmp1 =  strictSplit(separate[0].substr(0, find_rn), ": ");
		// 							if (tmp1.size())
		// 								std::copy(tmp1.begin(), tmp1.end(), std::back_inserter(var));
		// 							separate[0].erase(0, find_rn + 2);
		// 							find_rn = separate[0].find("\r\n");
		// 							if (find_rn == std::string::npos)
		// 							{
		// 								stringVect tmp = strictSplit(separate[0].substr(0, find_rn), ": ");
		// 								if (tmp.size())
		// 									std::copy(tmp.begin(), tmp.end(), std::back_inserter(var));
		// 							}
		// 						}
		// 					}
		// 					else
		// 						var = strictSplit(separate[0], ": ");
		// 					if (var.size())
		// 						(*it).getObjRequest().set_post_body(var, *(separate.begin() + 1));
		// 				}
		// 				std::cout << "BODY=====>[" << tmp << "]" << std::endl;
		// 			}
		// 			else if (is_boundaryEnd != std::string::npos)
		// 			{
		// 				stringVect separate;
		// 				std::string tmp = body.substr(0, is_boundaryEnd - 2);
		// 			 	size_t is_end = tmp.find("\r\n\r\n");
		// 				if (is_end != std::string::npos)
		// 				{
		// 					if (is_end + 4 == tmp.size())
		// 					{
		// 						tmp.erase(is_end);
		// 						separate.push_back(tmp);
		// 						separate.push_back("");
		// 					}
		// 				}
		// 				else
		// 					separate = strictSplit(tmp, "\r\n\r\n");
		// 				if (separate.size())
		// 					std::cout << " END{"<< tmp << "} ["<< separate[0] << "][" << separate[1] << "]" << std::endl;
		// 				if (separate.size() == 2)
		// 				{
		// 					stringVect var_line;
		// 					stringVect var;

		// 					size_t find_rn = separate[0].find("\r\n");
		// 					if (find_rn != std::string::npos)
		// 					{
		// 						while (find_rn != std::string::npos)
		// 						{
		// 							stringVect tmp1 =  strictSplit(separate[0].substr(0, find_rn), ": ");
		// 							if (tmp1.size())
		// 								std::copy(tmp1.begin(), tmp1.end(), std::back_inserter(var));
		// 							separate[0].erase(0, find_rn + 2);
		// 							find_rn = separate[0].find("\r\n");
		// 							if (find_rn == std::string::npos)
		// 							{
		// 								stringVect tmp = strictSplit(separate[0].substr(0, find_rn), ": ");
		// 								if (tmp.size())
		// 									std::copy(tmp.begin(), tmp.end(), std::back_inserter(var));
		// 							}
		// 						}
		// 					}
		// 					else
		// 						var = strictSplit(separate[0], ": ");
		// 					if (var.size())
		// 						(*it).getObjRequest().set_post_body(var, *(separate.begin() + 1));
		// 				}
		// 				std::cout << "\e[5m		TAPITRA       \e[0m" << std::endl;
		// 			}
		// 			else
		// 			{
		// 				// stringVect separate = strictSplit(body, limit);
		// 				// if (separate.size())
		// 				// 	std::cout << "["<< separate[0] << "][" << separate[1] << "]" << std::endl;
		// 				stringVect separate;
		// 				std::string tmp = body;
		// 			 	size_t is_end = tmp.find("\r\n\r\n");
		// 				if (is_end != std::string::npos)
		// 				{
		// 					if (is_end + 4 == tmp.size())
		// 					{
		// 						tmp.erase(is_end);
		// 						separate.push_back(tmp);
		// 						separate.push_back("");
		// 					}
		// 				}
		// 				else
		// 					separate = strictSplit(tmp, "\r\n\r\n");
		// 				if (separate.size() == 2)
		// 				{
		// 					stringVect var_line;
		// 					stringVect var;

		// 					size_t find_rn = separate[0].find("\r\n");
		// 					if (find_rn != std::string::npos)
		// 					{
		// 						while (find_rn != std::string::npos)
		// 						{
		// 							stringVect tmp1 =  strictSplit(separate[0].substr(0, find_rn), ": ");
		// 							if (tmp1.size())
		// 								std::copy(tmp1.begin(), tmp1.end(), std::back_inserter(var));
		// 							separate[0].erase(0, find_rn + 2);
		// 							find_rn = separate[0].find("\r\n");
		// 							if (find_rn == std::string::npos)
		// 							{
		// 								stringVect tmp = strictSplit(separate[0].substr(0, find_rn), ": ");
		// 								if (tmp.size())
		// 									std::copy(tmp.begin(), tmp.end(), std::back_inserter(var));
		// 							}
		// 						}
		// 					}
		// 					else
		// 						var = strictSplit(separate[0], ": ");
		// 					if (var.size())
		// 						(*it).getObjRequest().set_post_body(var, *(separate.begin() + 1));
		// 				}
		// 				std::cout << "BODY==NOT=END===>[" << body << "]" << std::endl;
		// 			}
		// 		}
		// 	}
		// }
		// (*it).getObjRequest().displayRequestMultipartData();
		// int fdCli = (*it).get_clientFD();
		// epoll.epoll_update_event(fdCli, EPOLLOUT);
	}
	catch(AExcept & e)
	{
		std::cerr << "\033[031mProcess Request exception catched : " << e._errMsg << "\033[0m" << std::endl;
		(*it).setCatchedResponse(e, fdFromRequest);
		epollOpt = EPOLLIN;
		if ((*it).getCategoryResponse() == REDIR_RSP || (*it).getCategoryResponse() == LDIR_RSP)
		{
			epoll.epoll_update_event(fdCli, EPOLLOUT);
			return;
		}
	}
	// std::cout << (*it).getObjRequest().getIsCgi() << " => FdFromRequest = " << fdFromRequest[FD_STDOUT] 
	// << "/" << fdFromRequest[FD_STDERR] << std::endl;
	if (fdFromRequest[FD_STDOUT] == -1)
	{
		handler.setProgramBreak(true);
		return;
	}
	if ((*it).getFdType() == HTML_FILE)
		handler.addToVectFileFd(it, fdFromRequest[FD_STDOUT]);
	else
	{
		handler.addToVectCgiFd(it, fdFromRequest[FD_STDOUT], fdFromRequest[FD_STDERR]);
		epoll.epoll_control(epollOpt, fdFromRequest[FD_STDERR]);
	}
	epoll.epoll_control(epollOpt, fdFromRequest[FD_STDOUT]);
}

void	sendingResponseToClient(VClientIt & it, int & fdCli, EpollCreation & epoll)
{
	int			responseCategory = (*it).getCategoryResponse();
	std::cout << "-->> \033[45mSendingResponseToClient : " << responseCategory << "\033[0m" << std::endl;

	if (responseCategory == LDIR_RSP)
		sendLdirResponse(it, fdCli, epoll); //Review for Listing Directory : maybe sending multiple response if the string is not fully sent
	else if (responseCategory == REDIR_RSP || responseCategory == SINGLE_RSP) //Response send only one time : redirection, POST and DELETE method
		sendSingleResponse(it, fdCli, epoll);
	else if ((*it).getPtrFileFd())
		sendHtmlFileResponse(it, fdCli, epoll);
	else if ((*it).getPtrCgiFd())
		sendCgiResponse(it, fdCli, epoll);
}

void	handleClientEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx)
{
	uint32_t	event = epoll.getEvent(eventIdx);
	VClientIt	it = handler.getClientIterator(i);
	int			fdCli = (*it).get_clientFD();
	
	if ((event & EPOLLIN) && (*it).getConnexionStat() == true)
		manageRequestFromClient(it, fdCli, epoll, handler);
	else if ((event & EPOLLOUT) && (*it).getConnexionStat() == true)
		sendingResponseToClient(it, fdCli, epoll);
	else
		throw std::runtime_error("HandleClientEvent error");
}

void	readFromFileFd(vectFileFdIt & it, EpollCreation & epoll, Handler & handler, int const & i)
{
	int			fdCli;
	int			bytesRead;
	char		buffer[_BUFFER];
	std::string	bufferString;
	
	std::cout << "\033[43mFileFd EPOLLIN : " << (*it)->getClientFd() << " ; OP = " << (*it)->getStatus() << "\033[0m" << std::endl;
	(*it)->manageFd();
	if ((*it)->getStatus() == OP_START)
		return;
	bytesRead = read((*it)->getFd(), buffer, _BUFFER);
	if (bytesRead <= 0)
	{
		(*it)->setStatus(OP_CLOSE);
		(*it)->clearBuffer();
		closeFileFdEvent(handler, i, epoll);
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
		fdCli = (*it)->getClientFd();
		if ((*it)->getStatus() == OP_OPEN)
			(*it)->setStatus(OP_START);
		epoll.epoll_update_event(fdCli, EPOLLOUT);
	}
	if ((*it)->getStatus() == OP_ONCE || (*it)->getStatus() == OP_CLOSE)
		closeFileFdEvent(handler, i, epoll);
}

void	handleFileEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx)
{
	vectFileFdIt	it = handler.getFileFdIterator(i);
	uint32_t		event = epoll.getEvent(eventIdx);

	if (event & EPOLLIN)
		readFromFileFd(it, epoll, handler, i);
	else if (event & EPOLLOUT)
	{
		std::cout << "			\033[43mFD EPOLLOUT FOR FILE IO" << "\033[0m" << std::endl;
		(*it)->set_endFilePost(true);
		closeFileFdEvent(handler, i, epoll);
	}
}

/*
	Cgi event is only reading through pipe no matter the method is
*/
void	handleCgiEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx)
{
	vectCgiFdIt	it = handler.getCgiFdIterator(i);
	uint32_t	event = epoll.getEvent(eventIdx);
	int			fdCli;
	int			bytesRead;
	char		buffer[_BUFFER];
	std::string	bufferString;

	if (event & EPOLLOUT)
	{
		std::cout << "Must be EPOLLIN";
		return; //Error on event selection
	}
	if ((*it)->getStatus() == OP_START) //See if necessary
		return;
	std::cout << "\033[43mCgiFd EPOLLIN : " << (*it)->getFd() << " ; OP = " << (*it)->getStatus() << "\033[0m" << std::endl;
	bytesRead = read((*it)->getFd(), buffer, _BUFFER);
	if (bytesRead <= 0)
	{
		(*it)->setStatus(OP_CLOSE);
		(*it)->clearBuffer();
		closeCgiFdEvent(handler, i, epoll);
		return ;
	}
	bufferString.append(buffer, bytesRead);
	(*it)->setBuffer(bufferString);
	std::cout << "				******** Buffer CGI = \n" << bufferString << std::endl;
	(*it)->bufferParser();
	(*it)->checkError();
	// (*it)->displayBufferParsed();
	if ((*it)->getStatus() == OP_OPEN)
	{
		(*it)->setStatus(OP_START);
		fdCli = (*it)->getClientFd();
		epoll.epoll_update_event(fdCli, EPOLLOUT);
	}
}

