/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:52 by barandri          #+#    #+#             */
/*   Updated: 2025/09/08 13:06:21 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "general.hpp"
#include "ErrorExcept.hpp"
#include "RedirExcept.hpp"

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

        void		generateStandardResponse(std::string const & body);
		void		generateCatchedResponse(AExcept & e, t_errPage & errPage);
		void		generateErrorResponse(ErrorExcept & e, t_errPage & errPage);
		void		generateRedirectResponse(RedirExcept & r);

		static void			initResponseUtils(std::string const & statPath, std::string const & errPath);
		static stringMap	getAllStatusCode();
};

#endif