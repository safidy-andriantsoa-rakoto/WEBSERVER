/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:02 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/19 14:04:53 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "general.hpp"
#include "../FileFd/FileFd.hpp"
#include "../Server/Server.hpp"
#include "../Response/Response.hpp"
#include "../Handler/Handler.hpp"

class Client : public Server
{
	private:
		Request		request;
	
		int			_receivedSize;
		int			_client_stat;
		int 		_clientFD;
		bool        _connect;
		std::string _clientRequest;
		std::string _boundary_sep;
		
		
		Response	_response;
		FileFd		_fileFD;
		size_t		_to_send;
		size_t		_total_send;
		
		Client();

		// void		processRequestWithConfig(Request & request);
		void		processRequestWithConfig();
		// void		checkContentLength(Request & request);
		void		checkContentLength();
		// void		buildRequestTarget(Request & request);
		void		buildRequestTarget();
		
		void		processLocationOption(MyLoc const & location, Request & request);
		void		checkMethodInLocation(stringVect & locationMethod, std::string const & method);
		void		processRedirection(MyLoc const & location, Request & request);

	public:
		Client(const Client& client);
		Client(Server const & server, const int &_clientFD, bool is_connect);
		Client& operator=(const Client& client);
		~Client();

		void		setToSendSize(size_t to_send);
		void		IncrementTotalSendSize(size_t to_send);
		size_t		getToSendSize();
		size_t		getTotalSendSize();
		bool		is_finishedSendContent();	
		
		Request&	getobjREquest();
		
		
		int         get_clientFD() const;
		std::string getClientRequest() const;
		bool        getConnexionStat();
		std::string getBoundaryLimit() const;
	

		FileFd      getFileFD() const;
		void        setFileFD(const FileFd &fileFd);

		void		linkClientAndFileFd(int const &fd, int const &fdCli, std::string const &filePath);

		Response	getResponse() const;
		void		updateResponseStatus(int const & stat);

		void		setCatchedResponse(AExcept & e);
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
		//Server main functionnality
		int processRequest(std::string const & clientRequest, uint32_t & opt, std::string & filePath);

		//Server utils
		vectLocationIt	findLongestMatch(std::string const & text, vectLocation & location);
		

};
