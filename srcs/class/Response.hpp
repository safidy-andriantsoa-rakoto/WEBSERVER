/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:52 by barandri          #+#    #+#             */
/*   Updated: 2025/08/04 15:57:36 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "general.hpp"

class Response
{
	private:
		int					_statusCode;
		std::string			_type;
		std::string			_body;
		std::string			_statusLine;
		stringMap			_header;
		std::string			_fullResponse;

		static stringMap	_allStatusCode;

		void				addDefaultHeader();

	public:
		Response();
		Response(std::string const & responsePath);
		Response(int const & statusCode, std::string type);
		Response(int const & statusCode, std::string const & responsePath, std::string const & type);
		Response(Response const & other);
		Response & operator=(Response const & other);
		~Response();

		std::string		getFullResponse();

		static void		initAllStatusCode(std::string const & filePath);
};

#endif