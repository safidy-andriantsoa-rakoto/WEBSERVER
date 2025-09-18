/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:02 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/18 11:39:50 by barandri         ###   ########.fr       */
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
		bool        _connect;
		std::string _clientRequest;

		Response	_response;
		FileFd		*_fileFD;

		Client();

	public:
		Client(const Client& client);
		Client(Server const & server, const int &_clientFD, bool is_connect);
		Client& operator=(const Client& client);
		~Client();

		int         get_clientFD() const;
		std::string getClientRequest() const;
		bool        getConnexionStat();

		FileFd		*getFileFD();
		int			getFileStatus();
		void        setFileFD(FileFd *fileFd);
		void		updateFileStatus(int const & status);

		Response &	getResponse();
		std::string	getFullResponse();
		std::string	getBodyResponse();
		int			getCategoryResponse();
		void		clearResponse();
		void		updateResponseStatus(int const & stat);
		void		setCatchedResponse(AExcept & e);
		void		setStandardResponse(int const & statusCode);
		void		setChunkedResponse(int const & statusCode);

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
