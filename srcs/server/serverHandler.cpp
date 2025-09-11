/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/09/11 08:03:57 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	handleServerEvent(Handler & handler, int const & i, EpollCreation & epoll)
{
	int fdc;

	while (1)
	{
		fdc = handler.getVectServer()[i].accept_client_connexion();
		std::cout << "		fdc = " << fdc << std::endl;
		if (fdc < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			else
			{
				perror("accept");
				break;
			}
		}
		epoll.no_blocking_fd(fdc);
		epoll.epoll_control(EPOLLIN , fdc);
		handler.addToVectClient(handler.getVectServer()[i], fdc);
		std::cout << "\033[42mNEW CLIENT JOIN=\033[0m" << fdc << "-->\033[046mSERVER=" << handler.getVectServer()[i].get_fdSocket() << "\033[0m" << std::endl;
	}
}

void	handleClientEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx)
{
	Client		client = handler.getVectClient()[i];
	int			fd = client.get_clientFD();
	uint32_t	event = epoll.getEvent(eventIdx);

	if ((event & EPOLLIN) && client.getConnexionStat() == true)
	{
		std::string client_request;
		int	fdServer = client.get_fdparent();
		int	fdFile;
		int	opt;
		Server	server = *(findFdInServer(handler.getVectServer(), fdServer));

		client.ClientRequest();
		client_request = client.getClientRequest();
		std::string filePath;
		std::cout << "-->> \033[40m	" << "Request, size = " << client_request.size() << "\033[0m" << std::endl;
		if (client_request.size() == 0)
			return ;
		fdFile = server.processRequest(client_request, fd, opt, filePath);
		handler.addToVectFileFd(fdFile, fd, filePath);
		epoll.epoll_control(opt, fdFile);
	}
	else if ((event & EPOLLOUT) && client.getConnexionStat() == true)
	{
		std::string type = Mime::getFileType(client.getFileFD().getFilePath());
		Response response(200, type);
		std::string	responseString;
		response.generateStandardResponse(client.getFileFD().getBuffer());
		responseString = response.getFullResponse();
		send_value(fd, responseString);
		epoll.epoll_update_event(fd, EPOLLIN);
	}
	else
	{
		epoll.epoll_clean(fd);
		handler.removeClient(i);
	}
}

void	handleFileEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & eventIdx)
{
	FileFd 		fileFd = handler.getVectFileFd()[i];
	uint32_t	event = epoll.getEvent(eventIdx);

	if (event & EPOLLIN) //Get request response
	{
		std::cout << "\033[43mFD EPOLLIN FOR FILE IO" << "\033[0m" << std::endl;
		char buf[_BUFFER];
		std::string buffer;
		size_t bytesRead;
		int fd;
		int	fdClient;

		fileFd.manageFd();
		while ((bytesRead = read(fileFd.getFileFd(), buf, _BUFFER)) > 0)
			buffer.append(buf, bytesRead);
		fileFd.setBuffer(buffer);
		fileFd.restoreFd();
		fd = fileFd.getFileFd();
		std::cout << "Fd = " << fd << std::endl;
		/*Change client to EPOLLOUT*/
		fdClient = fileFd.getClientFd();
		VClientIt it = findFdInClient(handler.getVectClient(), fdClient);
		epoll.epoll_update_event(fdClient, EPOLLOUT);
		(*it).setFileFD(fileFd);
		/**************************** */
		epoll.epoll_clean(fd);
		fileFd.closeAllOwnedFileFd();
		handler.removeFileFd(i);
		// std::cout << "HANDLE FILE IO DONE \n";
	}
	else if (event & EPOLLOUT)
	{
		std::cout << "\033[43mFD EPOLLOUT FOR FILE IO" << "\033[0m" << std::endl;
	}
	else
		std::cout << "ERROR ON EPOLL FILE IO\n";
}
