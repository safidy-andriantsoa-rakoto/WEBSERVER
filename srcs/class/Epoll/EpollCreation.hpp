/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollCreation.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:25:40 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/26 15:08:13 by barandri         ###   ########.fr       */
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
        void epoll_control(int _opt, int &clientFD);
        void epoll_clean(int &clientFD);
        int  get_fdEvent(int index);
        int  epoll_wait_event();
};
