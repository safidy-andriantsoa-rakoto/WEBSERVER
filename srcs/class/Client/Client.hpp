/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:02 by rsafidy-          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/09/19 21:11:56 by rsafidy-         ###   ########.fr       */
=======
/*   Updated: 2025/10/02 13:56:44 by rsafidy-         ###   ########.fr       */
>>>>>>> 6ae73645f3cc8449709d44042828db1faf1e59a2
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
<<<<<<< HEAD
		bool		_is_body_post;
		
=======

		Request		_request;
		Response	_response;
		FileFd		*_fileFD;

>>>>>>> 6ae73645f3cc8449709d44042828db1faf1e59a2
		Client();

		void		processRequestWithConfig();
		void		checkContentLength();
		void		buildRequestTarget();
		
		void		processLocationOption(MyLoc const & location, Request & request);
		void		checkMethodInLocation(stringVect & locationMethod, std::string const & method);
		void		processRedirection(MyLoc const & location, Request & request);

				//HTTP Method
		int			get_request();
		int			post_request(stringMap & header, std::string & md, std::string & boundary);
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
<<<<<<< HEAD
		//Server main functionnality
		int processRequest(std::string const & clientRequestContent, uint32_t & opt, std::string & filePath);
=======

		int processRequest(std::string const & clientRequest, uint32_t & opt, std::string & filePath);
>>>>>>> 6ae73645f3cc8449709d44042828db1faf1e59a2

		vectLocationIt	findLongestMatch(std::string const & text, vectLocation & location);

		int	creat_file_for_post(std::string path, uint32_t & opt);

		std::string currentDateTimeString();

};
