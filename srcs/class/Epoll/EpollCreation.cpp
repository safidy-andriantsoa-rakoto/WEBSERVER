/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollCreation.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:26:07 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/11/07 20:36:48 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EpollCreation.hpp"

EpollCreation &EpollCreation::operator=(const EpollCreation &ec)
{
    /* [?] : No egality Safidy ? */
    (void)ec;
    return (*this);
}

EpollCreation::EpollCreation()
{
    this->efds = epoll_create1(0);
    if (efds < 0)
    {
        std::string error(strerror(errno));
		throw std::runtime_error("\033[31mEpoll_create:" + error + "\033[0m");
    }
}

EpollCreation::~EpollCreation()
{
    if (this->efds > -1)
        close(this->efds);
}

void EpollCreation::epoll_control(uint32_t _opt, int const & clientFD)
{
    struct epoll_event client;
    client.events = _opt;
    client.data.fd = clientFD;

    if (epoll_ctl(this->efds, EPOLL_CTL_ADD, clientFD, &client) < 0)
    {
        std::string error(strerror(errno));
        throw std::runtime_error("\033[31m[ERROR]~epoll_ctl:" + error + "\033[0m");
    }
    std::cout << "\e[1;33mEpoll_ctrladd : " << clientFD << "\e[0m" << std::endl;
}

void EpollCreation::epoll_clean(int const & clientFD)
{
    std::cout << "          Epoll clean = " << clientFD << std::endl;

    if (epoll_ctl(efds, EPOLL_CTL_DEL, clientFD, NULL) < 0)
    {
        std::string error(strerror(errno));
        throw std::runtime_error("\033[31m[ERROR]~Epoll_ctl(clean):" + error + "\033[0m");
    }
    std::cout << "\e[1;33mEpoll_clean : " << clientFD << "\e[0m" << std::endl;
    if (clientFD > -1)
        close(clientFD);
}

int EpollCreation::get_fdEvent(int index)
{
    if (index < 0 || index > MAX_EVENT)
        throw std::runtime_error("\033[31m[ERROR]getFdEvent: Invalid index\033[0m");
    return (this->max_event[index].data.fd);
}

int EpollCreation::epoll_wait_event()
{
    int _nb = epoll_wait(this->efds, this->max_event, MAX_EVENT, 1000);
    if (_nb < 0)
    {
        std::string error(strerror(errno));
        throw std::runtime_error("\033[31m[ERROR]~Epoll_wait:" + error + "\033[0m");
    }
    return (_nb);
}

SEpollEvent& EpollCreation::getEvent_struct(int index)
{
    if (index < 0 || index > MAX_EVENT)
        throw std::runtime_error("\033[31m[ERROR]getEvent_struct: Invalid index\033[0m");
    return (this->max_event[index]);
}

void EpollCreation::epoll_update_event(int const &fd, uint32_t event)
{
    epoll_event up;
    up.data.fd = fd;
    up.events = event;

    // std::cout << "Epoll update event for " << fd << std::endl;
    if (epoll_ctl(this->efds, EPOLL_CTL_MOD, fd, &up) < 0)
    {
        std::string error(strerror(errno));
        error += " (" + convertIntToString(fd) + ")";
        throw std::runtime_error("\033[31m[ERROR]~Epoll_ctl(update):" + error + "\033[0m");
    }
}

uint32_t EpollCreation::getEvent(int const &index)
{
    if (index < 0 || index > MAX_EVENT)
        throw std::runtime_error("\033[31m[ERROR]getEvent: Invalid index\033[0m");
    return max_event[index].events;
}

