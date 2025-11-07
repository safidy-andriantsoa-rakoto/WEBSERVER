/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 11:12:25 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/11/07 20:59:02 by barandri         ###   ########.fr       */
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

