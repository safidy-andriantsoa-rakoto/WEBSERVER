/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/10/08 16:54:08 by rsafidy-         ###   ########.fr       */
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
	std::string limit = "\r\n\r\n";
	std::string body;
	std::string filePath;
	Request		tmpRequest;
	std::string limiter = "--";
	std::string limiterEnd = "--";
	
	tmpRequest = (*it).getobjREquest();
	(*it).ClientRequest();
	client_request = (*it).getClientRequest();
	// if (client_request.size() == 0)
	// {
	// 	(*it).clearFileFd();
	// 	// throw std::runtime_error("ManageRequestFromClient - request size = 0");
	// }
	try
	{
		fdFile = (*it).processRequest(client_request, fileFdOpt, filePath);// parsing POST process
		(*it).updateResponseCategory(STD_RSP);
	}
	catch(AExcept & e)
	{
		std::cerr << "\033[031mProcess Request exception catched : " << e._errMsg << "\033[0m" << std::endl;
		(*it).setCatchedResponse(e, fdFile, filePath);
		fileFdOpt = EPOLLIN;
		if ((*it).getCategoryResponse() == REDIR_RSP || (*it).getCategoryResponse() == LDIR_RSP)
		{
			epoll.epoll_update_event(fd, EPOLLOUT);
			return;
		}
	}
	if (fdFile != -1)
	{
		// std::cout << "\033[33m		CREATE FILEFD *\033[0m" << std::endl;
		handler.addToVectFileFd(it, fdFile, fd, filePath);
		epoll.epoll_control(fileFdOpt, fdFile);
		// std::cout << "\033[33m		CREATE FILEFD *\033[0m" << std::endl;
	}
	if ((*it).getFileFD() && (*it).getobjREquest().getMethod() == "POST")
	{
		// std::cout << " BODY ATO AAAANNN  ["<< (*it).getobjREquest().get_Body() << "]" << std::endl;
		(*it).getFileFD()->set_post_body((*it).getobjREquest().get_Body());
		(*it).getobjREquest().clear_body();
	}
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
	// std::cout <<"\e[45mResponse from the file : " << (*it).getFileFD()->getFilePath() << "\e[0m" << std::endl;
	// if (responseCategory == ERR_RSP && responseString.find("$status") != std::string::npos)
	// {
	// 	std::string	replace;

	// 	replace = convertIntToString(statusCode);
	// 	replace += " : " + (*(Response::_allStatusCode).find(replace)).second;
	// 		replaceString(responseString, replace, "$status");
	// }
	send_value(fd, responseString);
	if (fileStatus != OP_ONGOING && fileStatus != OP_START)
		epoll.epoll_update_event(fd, EPOLLIN);
	if (fileStatus == OP_START)
		(*it).updateFileStatus(OP_ONGOING);
	(*it).increaseFileSent(_BUFFER);
	if (fileStatus == OP_ONCE || fileStatus == OP_CLOSE)
		(*it).clearFileFd();
}

void	handleClientEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx)
{
	uint32_t	event = epoll.getEvent(eventIdx);
	VClientIt	it = handler.getClient(i);
	int			fd = (*it).get_clientFD();

	if ((event & EPOLLIN) && (*it).getConnexionStat() == true)
	{
		std::cout << "				EPOLLIN CLIENT" << std::endl;
		manageRequestFromClient(it, fd, epoll, handler);
	}
	else if ((event & EPOLLOUT) && (*it).getConnexionStat() == true)
	{
		std::cout << "				EPOLLOUT CLIENT" << std::endl;
		sendingResponseToClient(it, fd, epoll);
	}
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
	VClientIt		clIt = handler.getClient(i);
	// int				ClientFD = (*clIt).get_clientFD();

	if (event & EPOLLIN)
		readFromFileFd(it, epoll, handler, i);
	else if (event & EPOLLOUT)
	{
		//open new file: avec son nom filename="..."
		//if filename && fd file binary out  compare fd == fdfileMPFDT
		//{
		// parse and get body file
		//write binary file jusqua boundary end et close apres
		//}
		// else
		std::cout << "ATY QQQ		\033[43mFD EPOLLOUT FOR FILE IO-" << "\033[0m" << (*it)->getFileFd() << std::endl;
		// if ((*it)->getFileFd() != -1 && (*it)->get_fdMPFDT() < 0)
		// std::cout << "				FILEFDOUT eeee" << std::endl;
		if ((*it)->getFileFd() != -1)
		{
			std::string body;
			(*it)->manageFd();

			body = (*it)->get_FI_post_body();
			(*it)->remove_FI_body();
			size_t len =  write((*it)->getFileFd(), body.c_str(), body.size());
			if (len == (size_t)(-1))
				std::cout << "BLEM->" << strerror(errno) << std::endl;
			else
				(*it)->setFileTotalSize(len);
			std::cout << "manoratra->" << (*it)->getFileTotalSize() << std::endl;
		}
		// (*it)->get_FI_post_body()
		if ((*clIt).getobjREquest().get_c_length_received() == (*clIt).getobjREquest().get_content_length())
		{
			std::cout << "			VITA TOMPOKO!" << std::endl;
			std::cout << "  TOTAL_LENGHT=" << (*clIt).getobjREquest().get_content_length() << "| RECEIVED=" << (*clIt).getobjREquest().get_c_length_received() << std::endl;
			(*it)->set_endFilePost(true);
			// epoll.epoll_update_event(ClientFD, EPOLLOUT);
			closeFileEvent(handler, i, epoll);
		}
	}
		// if (!(*it)->get_FI_post_body().size() && (*clIt).getobjREquest().get_c_length_received() == (*clIt).getobjREquest().get_content_length())
}

