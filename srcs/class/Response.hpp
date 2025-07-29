/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:52 by barandri          #+#    #+#             */
/*   Updated: 2025/07/29 14:46:13 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "general.hpp"

class Response
{
	private:
		int			_statusCode;
		std::string	_type;
		std::string	_body;

		static stringMap	_allStatusCode;

	public:
		Response();
		Response(Response const & other);
		Response & operator=(Response const & other);
		~Response();

		int			getStatusCode() const;

		void		setResponseBody(std::string const & path);

		std::string	responseGenerator(std::string const & responsePath, int const & statusCode);

		static void	initAllStatusCode(std::string const & filePath);
};

#endif