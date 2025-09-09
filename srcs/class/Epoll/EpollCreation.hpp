/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollCreation.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:25:40 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/09 09:45:58 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "general.hpp"

class EpollCreation
{
    private:
        int                 efds;
        SEpollEvent         max_event[MAX_EVENT];

        EpollCreation(const EpollCreation& ec);
        EpollCreation& operator=(const EpollCreation& ec);
        
    public:
        EpollCreation();
        ~EpollCreation();
        void            epoll_control(int _opt, int &clientFD);
        void            epoll_clean(int &clientFD);
        int             get_fdEvent(int index);
        int             epoll_wait_event();
        SEpollEvent&    getEvent_struct(int index);
        void            epoll_update_event(int &fd, int event);
        void            no_blocking_fd(int fd);
        uint32_t        getEvent(int const &index);
};
