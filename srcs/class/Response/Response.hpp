/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:52 by barandri          #+#    #+#             */
/*   Updated: 2025/10/16 15:24:27 by barandri         ###   ########.fr       */
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

		int					_respStat;
		std::string			_originPath;
		std::string			_originBody;
		size_t				_pos;

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

		size_t				getPosition() const;
		void				setPosition(size_t const & newPos);

		int					getRespStat() const;
		void				setRespStat(int const & stat);

		std::string			getOriginPath() const;
		void				setOriginPath(std::string const & path);

		void				addHeader(std::string const & header, std::string const & content);
	
		int					getRespCategory() const;
		std::string			getFullResponse();

		std::string			getBody() const;
		std::string			getOriginBody() const;
		void				setOriginBody(std::string const & body);

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