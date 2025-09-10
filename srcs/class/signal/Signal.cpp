/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 11:12:55 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/10 11:28:04 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Signal.hpp"

void Signal::hander_signal(int sig)
{
    if (sig == SIGINT)
        signum = SIGINT;
}

int Signal::getSigNum() const
{
    return (signum);
}

int Signal::signum = -1;

Signal::Signal()
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, this->hander_signal);
}

Signal::~Signal(){}

