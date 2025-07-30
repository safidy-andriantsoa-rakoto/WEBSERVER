/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/07/30 20:12:23 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "general.hpp"

class Request
{
	private:
		std::string		_requestLine;
		std::string		_method;
		std::string		_requestTarget;
		std::string		_clientHttpVersion;
		std::string		_targetFileName;

		stringMap		_header;
		std::string		_body;

	public:
		Request();
		Request(Request const & other);
		Request & operator=(Request const & other);
		~Request();

		std::string	getRequestLine() const;
		std::string	getMethod() const;
		std::string	getRequestTarget() const;
		std::string	getHttpVersion() const;
		stringMap	getHeader() const;
		std::string	getBody() const;

		void    setRequestLine(std::string & set, stringMap const & locationMap, std::string const & generalRoot);
		void	setRequestTarget(std::string const & target, stringMap const & locationMap, std::string const & generalRoot);
		void    setBody(std::string & set);

		void    addHeader(std::string & field, std::string & content);

		void	displayRequest() const;

		void	getRequestBody(std::string const & requeststring, size_t const & pos);
		void	parseRequest(std::string const & requestString, stringMap const & locationMap, std::string const & generalRoot);
};

#endif