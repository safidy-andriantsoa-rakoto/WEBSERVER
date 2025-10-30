/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:07:07 by barandri          #+#    #+#             */
/*   Updated: 2025/10/30 11:09:26 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	closeCgiFdEvent(Handler & handler, int const & i, EpollCreation & epoll)
{
	std::cout << "Close CGI FD EVENT" << std::endl;
	vectCgiFdIt	itCgi = handler.getCgiFdIterator(i);

	if ((*itCgi)->getOutputFd() > 0)
		epoll.epoll_clean((*itCgi)->getOutputFd());
	if ((*itCgi)->getInputFd() > 0)
		epoll.epoll_clean((*itCgi)->getInputFd());
	handler.removeCgiFd(itCgi);
}

void	createCgiFd(VClientIt & it, Handler & handler, EpollCreation & epoll, int *fd, pid_t const & childPid)
{
	std::cout << "Create CGI FD" << std::endl;
	std::cout << fd[FD_STDIN] << " " << fd[FD_STDOUT] << " => " << childPid << std::endl;

	if ((*it).getFdType() == GET_PHP_SCRIPT)
	{
		handler.addToVectCgiFd(it, fd, childPid);
		epoll.epoll_control(EPOLLIN, fd[FD_STDOUT]);
	}
	else if ((*it).getFdType() == POST_PHP_SCRIPT)
	{
		handler.addToVectCgiFd(it, fd, childPid);
		epoll.epoll_control(EPOLLOUT, fd[FD_STDIN]);
		(*it).getPtrCgiFd()->setBufferWrite((*it).getObjRequest().getBody());
		int fdCli = (*it).get_clientFD();
		epoll.epoll_update_event(fdCli, EPOLLOUT);
	}
}

void	handleCgiError(Handler & handler, int const & index, EpollCreation & epoll, AExcept & err)
{
	vectCgiFdIt	itCgi = handler.getCgiFdIterator(index);
	int fdCli = (*itCgi)->getClientFd();
	VClientIt	it = findFdInClient(handler.getVectClient(), fdCli);

	std::cout << "Handle cgi error " << std::endl;
	(*it).setCatchedResponse(err);
	closeCgiFdEvent(handler, index, epoll);
	(*it).clearPtrCgiFd();
	epoll.epoll_update_event(fdCli, EPOLLOUT);
}

bool	hasSingleResponse(int const & respStat)
{
	if (respStat == REDIR_RSP || respStat == SINGLE_RSP || respStat == LDIR_RSP)
		return true;
	return false;
}

std::string	getIdentifier(int const & identifier)
{
	std::string	res;

	switch (identifier)
	{
		case SERVER: res = "SERVER"; break;
		case CLIENT: res = "SERVER"; break;
		case CGI_FD_OUTPUT: res = "CGI_FD_OUTPUT"; break;
		case CGI_FD_INPUT: res = "CGI_FD_INPUT"; break;
		default: res = "Unknown"; break;
	}
	return res;
}