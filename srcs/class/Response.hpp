/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:52 by barandri          #+#    #+#             */
/*   Updated: 2025/08/01 09:27:59 by barandri         ###   ########.fr       */
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

		static stringMap	_allStatusCode;

	public:
		Response();
		Response(Response const & other);
		Response & operator=(Response const & other);
		~Response();

		int			getStatusCode() const;

		void		setStatusCode(int const & statusCode);
		void		setType(std::string const & type);

		std::string getStatusLine(int const & statusCode);

		std::string	responseGenerator(std::string const & responsePath);
		std::string	responseGenerator(int const & statusCode, std::string const & responsePath, std::string const & type);

		std::string responseErrorGenerator(int const & statusCode, std::string const &responsePath); // For error

		static void	initAllStatusCode(std::string const & filePath);
};

#endif