/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/09/20 22:01:21 by rsafidy-         ###   ########.fr       */
=======
/*   Updated: 2025/10/02 11:53:34 by rsafidy-         ###   ########.fr       */
>>>>>>> 6ae73645f3cc8449709d44042828db1faf1e59a2
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "general.hpp"

class Request
{
	private:
		std::string	_requestLine;
		std::string	_method;
		std::string	_requestTarget;
		std::string	_originalRequestTarget;
		std::string	_clientHttp;

		stringMap	_header;
		// stringMap	_post_body;
		std::string	_body;

		MapSVectStr	_multipart_body;
		size_t		_request_length;
		bool		_autoIdx;
		bool		_is_CRLF_header;
		//for post
		size_t		_content_length;
		size_t		_c_length_received;

		void    	setRequestLine(std::string const & set);

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
		std::string&	getBody();
		stringMap	getHeader() const;
<<<<<<< HEAD
		stringMap	getpostInfo() const;
		
		void		setRequestBody(std::string const & requeststring, size_t const & pos);
=======
		const	MapSVectStr&	getpostMultipartData() const;
>>>>>>> 6ae73645f3cc8449709d44042828db1faf1e59a2

		void 		Del_space(std::string &data);

		void		set_is_CRLF_header(bool is_crlf);
		bool		get_is_CRLF_header();
		
		bool		getAutoIdx() const;
		void		setAutoIdx(bool const & autoIdx);

		void		setRequestSize(ssize_t size);
		ssize_t		getRequestSize();

		void		set_Body(const std::string& body);
		std::string&	get_Body();
		void		set_post_body(stringVect var, std::string content);

		void		setRequestTarget(std::string const & target);
	
		void		parseRequest(std::string const & requestString);

		void		parseBodyMultipartData(std::string const & requestString);

		void		displayRequest() const;

		void		displayRequestMultipartData();

		size_t 		get_c_length_received();
		void 		set_c_length_received(size_t received);

		size_t 		get_content_length();
};

#endif