/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:07:07 by barandri          #+#    #+#             */
/*   Updated: 2025/10/03 08:13:52 by barandri         ###   ########.fr       */
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
