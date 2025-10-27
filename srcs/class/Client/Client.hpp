/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 08:50:02 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/10/27 13:50:32 by barandri         ###   ########.fr       */
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
		std::string	filepath;

		bool		_modPostFile;
		int			_delay;
		time_t		_last_time;
		time_t		_time_now;

		Client();

		void		processRequestWithConfig();
		void		checkContentLength();
		void		buildRequestTarget();
		
		void		processLocationOption(MyLoc const & location, Request & request);
		void		checkMethodInLocation(stringVect & locationMethod, std::string const & method);
		void		processRedirection(MyLoc const & location, Request & request);

				//HTTP Method
		void		get_request(uint32_t & opt, int *fd, pid_t & childPid);
		void		stdGetRequest();
		void		cgiGetRequest(int *fd, pid_t & childPid);

		void		post_request(stringMap & header, uint32_t & opt, int *fd, pid_t & childPid);
		void		stdPostRequest(stringMap & header, uint32_t & opt);
		void		cgiPostRequest(int *fd, pid_t & childPid);
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

		int         get_clientFD() const;
		std::string getClientRequest() const;
		bool        getConnexionStat();
		std::string getBoundaryLimit() const;
		void		setBoundaryLimit(std::string boundary);
	
		int			getFdType() const;
		void		setFdType(int const & type);

		int			getResponseStat() const;
		void		setResponseStat(int const & stat);

		std::string	getRspOriginPath() const;
		void		setRspOriginPath(std::string const & path);

		void		setRspType(std::string const & filePath);

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
		void		setChunkedResponse();
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

		void			processRequest(std::string const & clientRequest, uint32_t & opt, int *fd, pid_t & childPid);

		vectLocationIt	findLongestMatch(std::string const & text, vectLocation & location);

		int				creat_file_for_post(std::string path, uint32_t & opt);

		std::string 	currentDateTimeString();
		bool			get_pathfile();
		void 			get_bounadary_from_header(stringMap &header);
		void			write_in_file_post();
		void			process_multipart_data();

		void			set_last_time_activity();
		void			set_time_activity();
		bool			is_client_timeout();
		void			set_delay(int delay);

};
