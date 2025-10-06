/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:02 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/10/06 14:43:29 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "general.hpp"
#include "../FileFd/FileFd.hpp"
#include "../Server/Server.hpp"
#include "../Response/Response.hpp"
#include "../CgiFd/CgiFd.hpp"

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
		CgiFd		*_cgiFD;

		int			_fd[2];

		Client();

		void		processRequestWithConfig();
		void		checkContentLength();
		void		buildRequestTarget();
		
		void		processLocationOption(MyLoc const & location, Request & request);
		void		checkMethodInLocation(stringVect & locationMethod, std::string const & method);
		void		processRedirection(MyLoc const & location, Request & request);

				//HTTP Method
		void		get_request(uint32_t & opt, int *fd);
		void		stdGetRequest(int *fd);
		void		cgiGetRequest(int *fd);

		void		post_request(stringMap & header, std::string & boundary, uint32_t & opt, int *fd);
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
		
		Request&	getObjRequest();
		
		
		int			*getFdTab();

		int         get_clientFD() const;
		std::string getClientRequest() const;
		bool        getConnexionStat();
		std::string getBoundaryLimit() const;
	

		FileFd		*getPtrFileFd();
		void        setPtrFileFd(FileFd *fileFd);
		void		clearPtrFileFd();
		void		increaseFileSent(size_t const & add);
		int			getFileStatus();
		void		updateFileStatus(int const & status);

		CgiFd		*getPtrCgiFd();
		void        setPtrCgiFd(CgiFd *cgiFd);
		void		clearPtrCgiFd();
		int			getCgiFdStatus();
		void		updateCgiFdStatus(int const & status);

		Response &	getObjResponse();
		std::string	getFullResponse();
		std::string	getBodyResponse();
		int			getCategoryResponse();
		int			getStatusCodeResponse();
		void		setStatusCodeResponse(int const & status);
		void		clearResponse();
		void		updateResponseCategory(int const & stat);
		void		setCatchedResponse(AExcept & e, int & fd);
		void		setStandardResponse();
		void		setChunkedResponse();
		void		setCgiChunkedResponse();

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

		void			processRequest(std::string const & clientRequest, uint32_t & opt, int *fd);

		vectLocationIt	findLongestMatch(std::string const & text, vectLocation & location);

		// void	epollAndHandlerCleanup(Handler & handler, EpollCreation & epoll);

};
