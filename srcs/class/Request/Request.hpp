/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/09/18 10:58:00 by rsafidy-         ###   ########.fr       */
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
		std::string	_clientHttp;

		stringMap	_header;
		stringMap	_postInfo;
		std::string	_body;
		stringVect 	_mulipart_body;
		size_t		_request_length;

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
		std::string	getRequestLine() const;
		std::string	getBody() const;
		stringMap	getHeader() const;
		stringMap	getpostInfo() const;

		void 		Del_space(std::string &data);

		void		setRequestSize(ssize_t size);

		ssize_t		getRequestSize();

		void		setRequestTarget(std::string const & target);
	
		void		parseRequest(std::string const & requestString);

		void		displayRequest() const;
};

#endif