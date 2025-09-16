/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:07:07 by barandri          #+#    #+#             */
/*   Updated: 2025/09/16 10:23:38 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

size_t	getFileSize(std::string const & path)
{
	struct stat	sf;

	if (stat(path.c_str(), &sf) == -1)
		throw std::runtime_error("Stat function error");
	return sf.st_size;
}

void	closeFileEvent(Handler & handler, int const & i, EpollCreation & epoll)
{
	vectFileFdIt	it = handler.getFileFd(i);
	int				fd;

	(*it).restoreFd();
	fd = (*it).getFileFd();
	epoll.epoll_clean(fd);
	close((*it).getStockFd());
	handler.removeFileFd(i);
	(*it).setStatus(OP_CLOSE);
}