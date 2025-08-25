/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:52 by barandri          #+#    #+#             */
/*   Updated: 2025/08/25 15:21:31 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "general.hpp"
#include "./tempClass/AConfig.hpp"

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
		static std::string	_defaultErrPage;

		void				addDefaultHeader();
		void				setStatusLine();
        void 				setToDefaultErrorPage();
		void				buildErrorPage(int const & statusCode, t_errPage & errPage);

	public:
		Response();
		Response(int const & statusCode);
		Response(int const & statusCode, std::string const & type);
		Response(Response const & other);
		Response & operator=(Response const & other);
		~Response();

		std::string	getFullResponse();

        void		responseGenerator(std::string const & filePath);
		void		responseErrorGenerator(int const & statusCode, t_errPage & errPage, std::string const & newUri);
		void		responseRedirect(std::string const & newUri);

		static void			initResponseUtils(std::string const & statPath, std::string const & errPath);
		static stringMap	getAllStatusCode();
};

#endif