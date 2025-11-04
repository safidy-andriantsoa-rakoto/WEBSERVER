/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:52 by barandri          #+#    #+#             */
/*   Updated: 2025/11/04 08:12:13 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "general.hpp"
#include "ErrorExcept.hpp"
#include "RedirExcept.hpp"
#include "LDirExcept.hpp"
#include "../CgiParser/CgiParser.hpp"
#include "../CgiFd/CgiFd.hpp"
#include "../Mime/Mime.hpp"

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

		std::fstream		_file;
		size_t				_contentLength;

		std::string			_originPath;
		std::string			_originBody;
		size_t				_pos;

		void				setStatusLine();
		void				addStatusCode(std::string & content);
		void				buildErrorPage(int const & statusCode, t_errPage & errPage, std::string & filePath, int & fdType);

	public:
		Response();
		Response(int const & statusCode);
		Response(int const & statusCode, std::string const & type);
		Response(Response const & other);
		Response & operator=(Response const & other);
		~Response();

		int					getStatusCode() const;

		std::string			getType() const;
		void				setType(std::string const & type);

		void				setStatusCode(int const & statusCode);

		size_t				getPosition() const;
		void				setPosition(size_t const & newPos);

		void				setFileStream(std::string const & path);

		size_t				getContentLength() const;
		void				setContentLength(size_t const & size);

		int					getRespStat() const;
		void				setRespStat(int const & stat);

		std::string			getOriginPath() const;
		void				setOriginPath(std::string const & path);

		void				addHeader(std::string const & header, std::string const & content);
	
		int					getRespCategory() const;
		void				setRespCategory(int const & category);

		std::string			getFullResponse();

		std::string			getBody() const;
		std::string			getOriginBody() const;
		void				setOriginBody(std::string const & body);

		void				clearResponse();

		void				closeFile();

        void				generateCgiSingleResponse(std::string const & body);/* [!] Need to erase */
        void				generateStandardResponse();
		void				generateChunkedResponse();
		void				generateCgiChunkedResponse(std::string const & body);
		void				generateCatchedResponse(AExcept & e, t_errPage & errPage, std::string & filePath, int & fdType);
		void				generateErrorResponse(ErrorExcept & e, t_errPage & errPage, std::string & filePath, int & fdType);
		void				generateRedirectResponse(RedirExcept & r);
		void				generateLDirResponse(LDirExcept & d);
		void				generateSingleResponse(int const & statusCode);

		static std::string	getReasonPhrase(int const & statusCode);
};

#endif