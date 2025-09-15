/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:02 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/15 08:23:14 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "general.hpp"
#include "../FileFd/FileFd.hpp"
#include "../Server/Server.hpp"
#include "../Response/Response.hpp"

class Client : public Server
{
	private:
		int			_client_stat;
		int 		_clientFD;

		std::string _clientRequest;
		bool        _connect;

		Response	_response;
		FileFd		_fileFD;

		Client();

	public:
		Client(const Client& client);
		Client(const int &_parentfd, const int &_clientFD, const SockAddrIn& _configSockAddrIn, bool is_socketDF);
		Client& operator=(const Client& client);
		Client(Server const & server, int const & fdCli);
		~Client();

		int         get_clientFD() const;
		std::string getClientRequest() const;
		bool        getConnexionStat();

		FileFd      getFileFD() const;
		void        setFileFD(const FileFd &fileFd);
		Response	getResponse() const;

		void		setCatchedResponse(AExcept & e, t_errPage & errPage);
		void		setStandardResponse(int const & statusCode);

		void        ClientRequest();

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
