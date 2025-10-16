/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:52 by barandri          #+#    #+#             */
/*   Updated: 2025/10/16 11:50:03 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "general.hpp"
#include "ErrorExcept.hpp"
#include "RedirExcept.hpp"
#include "LDirExcept.hpp"
#include "../CgiParser/CgiParser.hpp"

class Response
{
	private:
		int					_statusCode;
		std::string			_type;
		std::string			_body;
		std::string			_statusLine;
		stringMap			_header;
		std::string			_fullResponse;

		int					_respCategory;

		size_t				_sizeSent;

		static stringMap	_allStatusCode;

		void				setStatusLine();
		void				buildErrorPage(int const & statusCode, t_errPage & errPage, std::string & filePath, int & fdType);

	public:
		Response();
		Response(int const & statusCode);
		Response(int const & statusCode, std::string const & type);
		Response(Response const & other);
		Response & operator=(Response const & other);
		~Response();

		int					getStatusCode() const;
		void				setType(std::string const & type);
		void				setStatusCode(int const & statusCode);
		void				setCategory(int const & category);

		void				addHeader(std::string const & header, std::string const & content);
	
		int					getRespCategory() const;
		std::string			getFullResponse();
		std::string			getBody() const;

		void				clearResponse();

        void				generateStandardResponse(std::string const & body);
		void				generateChunkedResponse(std::string const & body, int const & status);
		void				generateCgiChunkedResponse(std::string const & body, stringMap const & header, int const & status);
		void				generateCatchedResponse(AExcept & e, t_errPage & errPage, std::string & filePath, int & fdType);
		void				generateErrorResponse(ErrorExcept & e, t_errPage & errPage, std::string & filePath, int & fdType);
		void				generateRedirectResponse(RedirExcept & r);
		void				generateLDirResponse(LDirExcept & d);
		void				generateSingleResponse(int const & statusCode);

		void				execCgiDefaultScript(int *fd);

		static void			initAllStatusCode(std::string const & statPath);
		static stringMap	getAllStatusCode();
};

#endif