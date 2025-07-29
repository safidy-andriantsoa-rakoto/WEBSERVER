/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:52 by barandri          #+#    #+#             */
/*   Updated: 2025/07/29 09:44:58 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "general.hpp"

class Response
{
	private:
		std::string	_statusLine;
		int			_statusCode;
		stringMap	_header;
		std::string	_body;

	public:
		Response();
		Response(Response const & other);
		Response & operator=(Response const & other);
		~Response();

		int			getStatusCode() const;
		std::string	getResponseFile() const;

		void		setStatusLine(int const & status, std::string const & reasonPhrase);
		void		setResponseBody(std::string const & path);

		void    	addHeader(std::string const & field, std::string const & content);
		void		addHeader(std::string const & field, int const & content);

		std::string	joinHeader() const;
		std::string	responseGenerator();
};

#endif