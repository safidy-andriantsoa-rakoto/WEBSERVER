/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:52 by barandri          #+#    #+#             */
/*   Updated: 2025/07/28 09:49:44 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "classUtils.hpp"
#include "define.hpp"

class Response
{
	private:
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

		void		setStatusCode(int const & status);
		void		setResponseFile(std::string const & path);

		std::string	responseGenerator() const;
};

#endif