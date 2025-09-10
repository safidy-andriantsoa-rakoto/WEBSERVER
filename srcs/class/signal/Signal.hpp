/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 11:12:25 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/10 11:34:41 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "general.hpp"

class Signal
{
    public:
        static int signum;
        Signal();
        ~Signal();
        static void    hander_signal(int sig);
        int     getSigNum() const;
};

