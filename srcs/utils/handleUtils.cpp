/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:07:07 by barandri          #+#    #+#             */
/*   Updated: 2025/09/23 08:34:45 by barandri         ###   ########.fr       */
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
	close((*it)->getStockFd());
	handler.removeFileFd(i);
}