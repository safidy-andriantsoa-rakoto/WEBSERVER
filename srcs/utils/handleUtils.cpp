/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:07:07 by barandri          #+#    #+#             */
/*   Updated: 2025/10/14 13:50:12 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	closeFileFdEvent(Handler & handler, int const & i, EpollCreation & epoll)
{
	int				fd;
	vectFileFdIt	it = handler.getFileFdIterator(i);

	(*it)->restoreFd();
	fd = (*it)->getFd();
	epoll.epoll_clean(fd);
	if ((*it)->getStockFd() != -1)
		close((*it)->getStockFd());
	handler.removeFileFd(i);
}

void	closeCgiFdEvent(Handler & handler, int const & i, EpollCreation & epoll)
{
	int			fd;
	vectCgiFdIt	it = handler.getCgiFdIterator(i);

	fd = (*it)->getFd();
	epoll.epoll_clean(fd);
	handler.removeCgiFd(i);
}

void	addFdInHandlerAndEpoll(VClientIt & it, Handler & handler, EpollCreation & epoll, int const & epollOpt, int *fdFromRequest)
{
	if (fdFromRequest[FD_STDOUT] < 0)
	{
		if (fdFromRequest[FD_STDOUT] == -2)
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

int	readCgiStdError(vectCgiFdIt & itCgi, EpollCreation & epoll)
{
	int			bytesRead;
	char		buffer[_BUFFER];
	std::string	bufferString;

	bytesRead = read((*itCgi)->getErrFd(), buffer, _BUFFER);
	if (bytesRead <= 0)
	{
		// std::cout << "Nothing to read for CGI Err \n";
		epoll.epoll_clean((*itCgi)->getErrFd());
		return 0;
	}
	bufferString.append(buffer, bytesRead);
	std::cout << "				***************BUFFER ERROR \n" << bufferString.size() << std::endl;
	return bytesRead;
}

void	handleCgiError(Handler & handler, int const & index, EpollCreation & epoll, AExcept & err, int const & identifier)
{
	int			fd[2] = {-1, -1};
	int			bytesRead = 0;
	vectCgiFdIt	itCgi = handler.getCgiFdIterator(index);
	VClientIt	it = findFdInClient(handler.getVectClient(), (*itCgi)->getClientFd());

	if (identifier == CGI_ERR_FD)
	{
		bytesRead = readCgiStdError(itCgi, epoll);
		if (!bytesRead)
			return;
	}
	(*it).setCatchedResponse(err, fd);
	if (fd[FD_STDOUT] == -1)
		return;
	epoll.epoll_clean((*itCgi)->getErrFd());
	epoll.epoll_clean((*itCgi)->getFd());
	handler.removeCgiFd(itCgi);
	if ((*it).getFdType() == HTML_FILE)
	{
		(*it).clearPtrCgiFd();
		handler.addToVectFileFd(it, fd[FD_STDOUT]);
	}
	else
	{
		handler.addToVectCgiFd(it, fd[FD_STDOUT], fd[FD_STDERR]);
		epoll.epoll_control(EPOLLIN, fd[FD_STDERR]);
	}
	epoll.epoll_control(EPOLLIN, fd[FD_STDOUT]);
}

bool	hasSingleResponse(int const & respStat)
{
	if (respStat == REDIR_RSP || respStat == SINGLE_RSP || respStat == LDIR_RSP)
		return true;
	return false;
}