/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/10/20 10:27:07 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "general.hpp"
#include "../CgiParser/CgiParser.hpp"

class Request
{
	private:
		std::string	_requestLine;
		std::string	_method;
		std::string	_requestTarget;
		std::string	_originalRequestTarget;
		std::string	_clientHttp;

		stringMap	_header;
		std::string	_body;

		size_t		_request_length;
		bool		_autoIdx;
	
		//for post
		MapSVectStr	_multipart_body;
		size_t		_content_length;
		size_t		_c_length_received;

		bool		_isCgi;
		CgiParser	_cgiParser;

		void    	setRequestLine(std::string const & set);
		void		setRequestBody(std::string const & requeststring, size_t const & pos);

	public:
		Request();
		Request(Request const & other);
		Request & operator=(Request const & other);
		~Request();

		std::string	getHttp() const;
		std::string	getMethod() const;
		std::string	getRequestTarget() const;
		std::string	getOriginalRequestTarget() const;
		std::string	getRequestLine() const;
		std::string	getBody() const;
		stringMap	getHeader() const;

		bool		getIsCgi() const;
		void		setIsCgi(bool const & condition);

		CgiParser	getCgiClass() const;
		void		processCgiClass(std::string const & target, std::string const & rootDirectory);
		void		setCgiClass(CgiParser const & cgi);
		void		execCgiClass();
		void		clearCgiClass();

		const	MapSVectStr&	getpostMultipartData() const;

		void 			Del_space(std::string &data);
		
		bool			getAutoIdx() const;
		void			setAutoIdx(bool const & autoIdx);

		void			setRequestSize(ssize_t size);
		ssize_t			getRequestSize();

		void			set_Body(const std::string& body);
		std::string&	get_Body();
		void			clear_body();
		void			set_post_body(stringVect var, std::string content);

		void			setRequestTarget(std::string const & target);
	
		void			parseRequest(std::string const & requestString);

		void			displayRequest() const;


		void		displayRequestMultipartData();

		size_t 		get_c_length_received();
		void 		set_c_length_received(size_t received);

		size_t 		get_content_length();
		void 		set_content_length(size_t size);
};

#endif