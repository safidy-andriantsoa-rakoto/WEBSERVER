/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfig.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:57:41 by barandri          #+#    #+#             */
/*   Updated: 2025/08/12 21:58:22 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AConfig.hpp"

int			AConfig::getServPort() const
{
    return _port;
}

std::string	AConfig::getServHost() const
{
    return _host;
}

stringVect	AConfig::getServName() const
{
    return _serverName;
}

t_errPage	AConfig::getErrPage() const
{
    return _errPage;
}

vectLocation AConfig::getClassLocation() const
{
    return _location;
}
