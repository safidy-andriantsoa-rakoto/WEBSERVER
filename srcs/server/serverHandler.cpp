/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/09/10 11:43:47 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	handleClient(Handler & handler, int const & index, EpollCreation & epoll)
{
	// Server	server = handler.getVectServer()[index];
	int fdc;
	// std::cout << "		fdc=" << fdc << std::endl;
	while ((fdc = handler.getVectServer()[index].accept_client_connexion()) > 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			break;
		else
		{
			epoll.no_blocking_fd(fdc);
			epoll.epoll_control(EPOLLIN , fdc);
			handler.addToVectClient(handler.getVectServer()[index], fdc);
			std::cout << "\033[42mNEW CLIENT JOIN=\033[0m" << fdc << "-->\033[046mSERVER=" << handler.getVectServer()[index].get_fdSocket() << "\033[0m" << std::endl;
		}
	}
}

void	handleRequest(Handler & handler, int const & index, EpollCreation & epoll, int const & eventIndex)
{
	Client client = handler.getVectClient()[index];
	int fd = client.get_clientFD();
	if (epoll.getEvent(eventIndex) & EPOLLIN)
	{
		if (client.getConnexionStat())
		{
			std::cout << "\033[45mFD EVENT:" << fd << "\033[0m" << std::endl;
			std::string client_request;
			int	fdServer = client.get_fdparent();
			int	fdFile;
			int	opt;
			Server	server = *(findFdInServer(handler.getVectServer(), fdServer));

			try
			{
				client.ClientRequest();
				client_request = client.getClientRequest();
				std::string filePath;
				std::cout << "-->> \033[40m	" << "Client request on treatment, size = " << client_request.size() << "\033[0m" << std::endl;
				if (client_request.size() < 100)
					std::cout << client_request << std::endl;
				fdFile = server.processRequest(client_request, fd, opt, filePath);
				handler.addToVectFileFd(fdFile, fd, filePath);
				epoll.epoll_control(opt, fdFile);

			}
			catch(const std::exception& e)
			{
				std::cout << "Error on handleRequest : \n" << e.what() << std::endl;
				epoll.epoll_clean(fd);
				handler.removeClient(index);
			}	
		}
		else
		{
			epoll.epoll_clean(fd);
			handler.removeClient(index);
		}
	}
	else if (epoll.getEvent(index) & EPOLLOUT)
	{
		if (client.getConnexionStat())
		{
			std::string type = Mime::getFileType(client.getFileFD().getFilePath());
			Response response(200, type);
			std::string	responseString;
			response.generateStandardResponse(client.getFileFD().getBuffer());
			responseString = response.getFullResponse();
			send_value(fd, responseString);
			epoll.epoll_update_event(fd, EPOLLIN); //Ampiana ve satria lasa mandefa reponse foana ilay server?
		}
		else
		{
			epoll.epoll_clean(fd);
			handler.removeClient(index);
		}
	}
}

void	handleFileIO(Handler & handler, int const & index, EpollCreation & epoll, int const & evIndex)
{
	FileFd fileFd = handler.getVectFileFd()[index];

	if (epoll.getEvent(evIndex) & EPOLLIN) //Get request response
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
		// fileFd.closeAllOwnedFileFd();
		handler.removeFileFd(index);
		std::cout << "HANDLE FILE IO DONE \n";
	}
	else if (epoll.getEvent(evIndex) & EPOLLOUT)
	{
		
		std::cout << "\033[43mFD EPOLLOUT FOR FILE IO" << "\033[0m" << std::endl;
	}
	else
		std::cout << "ERROR ON EPOLL FILE IO\n";
}
