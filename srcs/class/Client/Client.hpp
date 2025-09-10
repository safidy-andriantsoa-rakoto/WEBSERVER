/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:02 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/10 09:31:02 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "general.hpp"
#include "../FileFd/FileFd.hpp"
#include "../Server/Server.hpp"

class Client
{
    private:
        int _client_stat;
        int _parentFD;
        int _clientFD;
        FileFd  _fileFD;
        SockAddrIn  _configSockAddrIn;
        std::string _clientRequest;
        bool        _connect;
        Client();

    public:
        Client& operator=(const Client& client);
        Client(const Client& client);
        Client(Server const & server, int const & fdCli);
        ~Client();
        Client(const int &_parentfd, const int &_clientFD, const SockAddrIn& _configSockAddrIn, bool is_socketDF);
        int         get_clientFD() const;
        int         get_fdparent() const;
        void        ClientRequest();
        std::string getClientRequest() const;
        bool        getConnexionStat();
        void        setFileFD(const FileFd &fileFd);
        FileFd      getFileFD();
        class Deconnect: public std::exception
        {
            public:
                const char *what() const throw();
        };
        class RecvError: public std::exception
        {
            public:
                const char *what() const throw();
        };
};
