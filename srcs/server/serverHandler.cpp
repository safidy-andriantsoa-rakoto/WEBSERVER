/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:34:53 by barandri          #+#    #+#             */
/*   Updated: 2025/09/09 13:26:01 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	handleClient(Handler & handler, int const & index, EpollCreation & epoll)
{
	Server	server = handler.getVectServer()[index];
	int fdc;

	while ((fdc = server.accept_client_connexion()) > 0)
	{
		// std::cout << "\033[42mNEW CLIENT JOIN=\033[0m" << fdc << "-->\033[046mSERVER=" << server.get_fdSocket() << "\033[0m" << std::endl;
		epoll.no_blocking_fd(fdc);
		epoll.epoll_control(EPOLLIN , fdc);
		handler.addToVectClient(server, fdc);
 	}
}

void	handleRequest(Handler & handler, int const & index, EpollCreation & epoll, int const & eventIndex)
{
	Client client = handler.getVectClient()[index];
	int fd = client.get_clientFD();
	if ((epoll.getEvent(eventIndex) & EPOLLIN) && client.getConnexionStat())
	{
		std::string client_request;
		int	fdServer = client.get_fdparent();
		int	fdFile;
		int	opt;
		Server	server = *(findFdInServer(handler.getVectServer(), fdServer));

		std::cout << "\033[45mFD EVENT:" << fd << "\033[0m" << std::endl;
		try
		{
			client.ClientRequest();
			client_request = client.getClientRequest();
			std::string filePath;
			std::cout << "-->> \033[40m	" << "Client request on treatment" << "\033[0m" << std::endl;
			fdFile = server.processRequest(client_request, fd, opt, filePath);
			handler.addToVectFileFd(fdFile, fd, filePath);
			epoll.epoll_control(opt, fdFile);

		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			std::cout << "Error on handleRequest : \n" << e.what() << std::endl;
			epoll.epoll_clean(fd);
			handler.removeClient(index);
		}
	}
	else if ((epoll.getEvent(index) & EPOLLOUT) && client.getConnexionStat())
	{
		std::string type = Mime::getFileType(client.getFileFD().getFilePath());
		Response response(200, type);
		std::string	responseString;
		response.generateStandardResponse(client.getFileFD().getBuffer());
		responseString = response.getFullResponse();
		send_value(fd, responseString);
		handler.removeFileFd(index);
	}
	else
	{
		epoll.epoll_clean(fd);
		handler.removeClient(index);
	}
}

void	handleFileIO(Handler & handler, int const & index, EpollCreation & epoll, int const & evIndex)
{
	FileFd fileFd = handler.getVectFileFd()[index];

	if (epoll.getEvent(evIndex) & EPOLLIN) //Get request
	{
		std::cout << "\033[43mFD EPOLLIN FOR FILE IO" << "\033[0m" << std::endl;
		char buf[_BUFFER];
		fileFd.manageFd();
		if (read(fileFd.getStockFd(), buf, _BUFFER == -1))
			throw std::runtime_error("Error on reading");
		fileFd.setBuffer(buf);
		fileFd.closeAllOwnedFileFd();
		int fd= fileFd.getClientFd();
		epoll.epoll_update_event(fd, EPOLLOUT);
		VClientIt it = findFdInClient(handler.getVectClient(), fileFd.getClientFd());
		(*it).setFileFD(fileFd);
	}
	else if (epoll.getEvent(evIndex) & EPOLLOUT) //Post or delete request
	{
		std::cout << "\033[43mFD EPOLLOUT FOR FILE IO" << "\033[0m" << std::endl;
	}
	else
		std::cout << "ERROR ON EPOLL FILE IO\n";
}
