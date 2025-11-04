/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventHandlingUtils.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:07:07 by barandri          #+#    #+#             */
/*   Updated: 2025/11/04 14:03:56 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	closeCgiFdEvent(Handler & handler, int const & i, EpollCreation & epoll)
{
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

void	manageCgiAfterProcessingRequest(VClientIt & it, EpollCreation & epoll, Handler & handler, int *fdCgi, pid_t childPid)
{
	int	fdCli = (*it).get_clientFD();
	if (fdCgi[FD_STDOUT] == -2)
	{
		handler.setProgramBreak(true);
		handler.setIsChildProcess(true);
		return;
	}
	if (fdCgi[FD_STDOUT] > 0)
		createCgiFd(it, handler, epoll, fdCgi, childPid);
	if ((*it).getFdType() == POST_PHP_SCRIPT)
		epoll.epoll_update_event(fdCli, EPOLLOUT);
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