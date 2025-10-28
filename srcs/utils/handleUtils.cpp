/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:07:07 by barandri          #+#    #+#             */
/*   Updated: 2025/10/28 09:39:24 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	closeCgiFdEvent(Handler & handler, int const & i, EpollCreation & epoll, int const & fd)
{
	std::cout << "Close CGI FD EVENT" << std::endl;
	epoll.epoll_clean(fd);
	handler.removeCgiFd(i);
}

void	createCgiFd(VClientIt & it, Handler & handler, EpollCreation & epoll, int *fd, pid_t const & childPid)
{
	std::cout << "Create CGI FD" << std::endl;
	std::cout << fd[FD_STDIN] << " " << fd[FD_STDOUT] << " " << fd[FD_STDERR] << " => " << childPid << std::endl;

	if ((*it).getFdType() == GET_PHP_SCRIPT)
	{
		handler.addToVectCgiFd(it, fd, childPid);
		epoll.epoll_control(EPOLLIN, fd[FD_STDOUT]);
		// epoll.epoll_control(EPOLLIN, fd[FD_STDERR]);
	}
	else if ((*it).getFdType() == POST_PHP_SCRIPT)
	{
		handler.addToVectCgiFd(it, fd, childPid);
			epoll.epoll_control(EPOLLOUT, fd[FD_STDIN]);
		int fdCli = (*it).get_clientFD();
		epoll.epoll_update_event(fdCli, EPOLLOUT);
		(*it).getPtrCgiFd()->setBufferWrite((*it).getObjRequest().getBody());
	}
}

int	readCgiStdError(vectCgiFdIt & itCgi, EpollCreation & epoll)
{
	int			bytesRead;
	char		buffer[_BUFFER];
	std::string	bufferString;

	bytesRead = read((*itCgi)->getErrFd(), buffer, _BUFFER);
	if (bytesRead <= 0)
	{
		std::cout << "Nothing to read for CGI ERROR \n";
		epoll.epoll_clean((*itCgi)->getErrFd());
		(*itCgi)->setErrFd(-1);
		return 0;
	}
	bufferString.append(buffer, bytesRead);
	std::cout << "				***************BUFFER ERROR \n" << bufferString << std::endl;
	std::cout << "				***************BUFFER ERROR END" << std::endl;
	if (bufferString.find("Valgrind") != std::string::npos)
	{
		std::cout << "Valgrind Error \n";
		epoll.epoll_clean((*itCgi)->getErrFd());
		(*itCgi)->setErrFd(-1);
		return 0;	
	}
	return bytesRead;
}

void	handleCgiError(Handler & handler, int const & index, EpollCreation & epoll, AExcept & err, int const & identifier)
{
	int			bytesRead = 0;
	vectCgiFdIt	itCgi = handler.getCgiFdIterator(index);
	int fdCli = (*itCgi)->getClientFd();
	VClientIt	it = findFdInClient(handler.getVectClient(), fdCli);

	/*Not needed anymore*/
	if (identifier == CGI_FD_ERR)
	{
		bytesRead = readCgiStdError(itCgi, epoll);
		if (!bytesRead)
			return;
	}
	std::cout << "Handle cgi error " << std::endl;
	(*it).setCatchedResponse(err);
	if ((*itCgi)->getOutputFd() > 0)
		epoll.epoll_clean((*itCgi)->getOutputFd());
	if ((*itCgi)->getErrFd() > 0)
		epoll.epoll_clean((*itCgi)->getErrFd());
	handler.removeCgiFd(itCgi);
	(*it).clearPtrCgiFd();
	epoll.epoll_update_event(fdCli, EPOLLOUT);
}

bool	hasSingleResponse(int const & respStat)
{
	if (respStat == REDIR_RSP || respStat == SINGLE_RSP || respStat == LDIR_RSP)
		return true;
	return false;
}