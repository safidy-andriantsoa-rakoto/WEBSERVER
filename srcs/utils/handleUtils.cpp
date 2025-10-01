/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:07:07 by barandri          #+#    #+#             */
/*   Updated: 2025/10/01 08:48:32 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	closeFileEvent(Handler & handler, int const & i, EpollCreation & epoll)
{
	vectFileFdIt	it = handler.getFileFd(i);
	int				fd;

	(*it)->restoreFd();
	fd = (*it)->getFileFd();
	epoll.epoll_clean(fd);
	if ((*it)->getStockFd() != -1)
		close((*it)->getStockFd());
	handler.removeFileFd(i);
}
