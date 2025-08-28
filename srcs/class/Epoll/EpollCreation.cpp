/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollCreation.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:26:07 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/28 14:54:35 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EpollCreation.hpp"

EpollCreation &EpollCreation::operator=(const EpollCreation &ec)
{
    (void)ec;
    return (*this);
}

EpollCreation::EpollCreation()
{
    this->efds = epoll_create1(0);
    if (efds < 0)
		throw std::runtime_error("Error on poll create");
}

EpollCreation::~EpollCreation()
{
    if (this->efds > -1)
        close(this->efds);
}

void EpollCreation::epoll_control(int _opt, int &clientFD)
{
    struct epoll_event client;
    client.events = _opt;
    client.data.fd = clientFD;
    if (epoll_ctl(this->efds, EPOLL_CTL_ADD, clientFD, &client) < 0)
    {
        perror("add ctl:");
        throw std::runtime_error("epoll_ctl_add error ee");
    }
}

void EpollCreation::epoll_clean(int &clientFD)
{
    if (epoll_ctl(efds, EPOLL_CTL_DEL, clientFD, NULL) < 0)
        perror ("EPOLL_CLEAN");
    if (clientFD > -1)
        close(clientFD);
}

int EpollCreation::get_fdEvent(int index)
{
    if (index < 0)
        return (-1);

    return (this->max_event[index].data.fd);
}

int EpollCreation::epoll_wait_event()
{
    return (epoll_wait(this->efds, this->max_event, MAX_EVENT, -1));
}

SEpollEvent EpollCreation::getEvent_struct(int index)
{
    return (this->max_event[index]);
}
