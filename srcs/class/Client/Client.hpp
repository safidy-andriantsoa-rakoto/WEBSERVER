/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:02 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/30 08:05:04 by barandri         ###   ########.fr       */
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
		int			_receivedSize;
		int			_client_stat;
		int 		_clientFD;
		bool        _connect;
		std::string _clientRequest;
		std::string _boundary_sep;
		size_t		_to_send;
		size_t		_total_send;

		Request		_request;
		Response	_response;
		FileFd		*_fileFD;

		Client();

		void		processRequestWithConfig();
		void		checkContentLength();
		void		buildRequestTarget();
		
		void		processLocationOption(MyLoc const & location, Request & request);
		void		checkMethodInLocation(stringVect & locationMethod, std::string const & method);
		void		processRedirection(MyLoc const & location, Request & request);

				//HTTP Method
		int			get_request();
		int			post_request(stringMap & header, std::string & boundary, uint32_t & opt);
		void		delete_request();

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
	

		FileFd		*getFileFD();
		int			getFileStatus();
		void        setFileFD(FileFd *fileFd);
		void		clearFileFd();
		void		updateFileStatus(int const & status);
		void		increaseFileSent(size_t const & add);

		Response &	getResponse();
		std::string	getFullResponse();
		std::string	getBodyResponse();
		int			getCategoryResponse();
		int			getStatusCodeResponse();
		void		clearResponse();
		void		updateResponseCategory(int const & stat);
		void		setCatchedResponse(AExcept & e, int & fd, std::string & filePath);
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

		int processRequest(std::string const & clientRequest, uint32_t & opt, std::string & filePath);

		vectLocationIt	findLongestMatch(std::string const & text, vectLocation & location);

};
