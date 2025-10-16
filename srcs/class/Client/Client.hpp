/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:02 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/10/16 11:16:37 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "general.hpp"
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
		CgiFd		*_cgiFD;
		int			_fdType;
		std::string filepath;

		Client();

		void		processRequestWithConfig();
		void		checkContentLength();
		void		buildRequestTarget();
		
		void		processLocationOption(MyLoc const & location, Request & request);
		void		checkMethodInLocation(stringVect & locationMethod, std::string const & method);
		void		processRedirection(MyLoc const & location, Request & request);

				//HTTP Method
		void		get_request(uint32_t & opt, int *fd, std::string & content);
		void		stdGetRequest(std::string & content);
		void		cgiGetRequest(int *fd);

		void		post_request(stringMap & header, uint32_t & opt, int *fd);
		void		stdPostRequest(stringMap & header, uint32_t & opt);
		void		cgiPostRequest(int *fd);
		void		delete_request();

		bool		_modPostFile;

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

		int         get_clientFD() const;
		std::string getClientRequest() const;
		bool        getConnexionStat();
		std::string getBoundaryLimit() const;
		void		setBoundaryLimit(std::string boundary);
	
		int			getFdType() const;
		void		setFdType(int const & type);

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
		void		setCatchedResponse(AExcept & e);
		void		setStandardResponse(std::string const & content);
		void		setChunkedResponse(std::string const & content, std::string const & type, int const & status);
		void		setCgiChunkedResponse();
		void		setSingleResponse(int const & statusCode);

		void        receiveClientRequest();

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

		void			processRequest(std::string const & clientRequest, uint32_t & opt, int *fd, std::string & content);

		vectLocationIt	findLongestMatch(std::string const & text, vectLocation & location);

		int				creat_file_for_post(std::string path, uint32_t & opt);

		std::string 	currentDateTimeString();

		void	switch_modPostFile(bool stat);
		bool	modPostFile_stat();

};
