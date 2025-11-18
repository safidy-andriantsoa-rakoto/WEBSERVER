/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/11/17 15:36:32 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "general.hpp"
#include "../CgiParser/CgiParser.hpp"
#include "../Response/ErrorExcept.hpp"

class Request
{
	private:
		std::string				_partRequest;
			
		std::string				_requestLine;
		std::string				_method;
		std::string				_requestTarget;
		std::string				_originalRequestTarget;
		std::string				_clientHttp;
			
		stringMap				_header;
		std::string				_body;
			
		size_t					_request_length;
		bool					_autoIdx;
		std::string				_index;
			
		MapSVectStr				_multipart_body;
		size_t					_content_length;
		size_t					_c_length_received;
			
		bool					_isCgi;
		CgiParser				_cgiParser;
		bool					_is_CRLF_header;
			
		void    				setRequestLine(std::string const & set);
		void					addRequestBody(std::string const & requeststring, size_t const & pos);

	public:
		Request();
		Request(Request const & other);
		Request & operator=(Request const & other);
		~Request();

		std::string				getHttp() const;
		std::string				getMethod() const;
		std::string				getRequestTarget() const;
		std::string				getRequestLine() const;
		std::string				getBody() const;
		stringMap				getHeader() const;

		std::string				getOriginalRequestTarget() const;
		void					setOriginalRequestTarget(std::string const & add);

		void					setPartRequest(std::string const & content);
		std::string				getPartRequest() const;

		bool					getIsCgi() const;
		void					setIsCgi(bool const & condition);

		CgiParser				getCgiParser() const;
		void					processCgiClass(std::string const & execPath, std::string const & root, int const & port);
		void					setCgiClass(CgiParser const & cgi);
		void					execCgiClass();
		void					clearCgiClass();

		const	MapSVectStr&	getpostMultipartData() const;

		void 					Del_space(std::string &data);

		bool					getAutoIdx() const;
		void					setAutoIdx(bool const & autoIdx);

		std::string				getIndex() const;
		void					setIndex(std::string const & index);

		void					setRequestSize(ssize_t size);
		ssize_t					getRequestSize();

		void					set_Body(const std::string& body);
		std::string&			get_Body();
		void					clear_body();
		void					set_post_body(stringVect var, std::string content);

		void					setRequestTarget(std::string const & target);

		void					parseRequest();

		void					displayRequest() const;

		void					displayRequestMultipartData();

		size_t 					get_c_length_received();
		void 					set_c_length_received(size_t received);

		size_t 					get_content_length();
		void 					set_content_length(size_t size);

		bool					checkContentReceived() const;

		void					delete_range_body_request(int start, int end);

		void					set_is_CRLF_header(bool is_crlf);
		bool					get_is_CRLF_header();
};

#endif