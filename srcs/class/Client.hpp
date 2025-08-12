/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:02 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/12 10:03:09 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "general.hpp"

class Client
{
    private:
        int _parentFD;
        int _clientFD;
        SockAddrIn  _configSockAddrIn;
        bool        _server_socket;
        Client();
        
    public:
        Client& operator=(const Client& client);
        Client(const Client& client);
        ~Client();
        Client(const int &_parentfd, const int &_clientFD, const SockAddrIn& _configSockAddrIn, bool is_socketDF);
        int         get_clientFD() const;
        int         get_parent() const;
        std::string ClientRequest();
};
