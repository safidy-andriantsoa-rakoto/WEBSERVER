/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollCreation.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:25:40 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/10/22 14:18:50 by barandri         ###   ########.fr       */
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

        void            epoll_control(uint32_t _opt, int const & fd);
        void            epoll_clean(int const & fd);
        int             get_fdEvent(int index);
        int             epoll_wait_event();
        SEpollEvent&    getEvent_struct(int index);
        void            epoll_update_event(int const & fd, uint32_t event);
        uint32_t        getEvent(int const &index);
};
