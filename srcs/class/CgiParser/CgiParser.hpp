/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:11:42 by barandri          #+#    #+#             */
/*   Updated: 2025/11/09 13:52:37 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIPARSER_HPP
# define CGIPARSER_HPP

#include "general.hpp"

class CgiParser
{
	private:
		std::string _reqMethod;
		std::string _scriptFileName;
		std::string	_contentType;
		size_t		_contentLength;
		
		std::string	_execPath;
		stringVect	_envpVect;
		char**		_envp;

		public:
		CgiParser();
		CgiParser(CgiParser const & other);
		CgiParser& operator=(CgiParser const & other);
		~CgiParser();

		char**		getEnvp();
		
		void    	parseCgiRequestTarget(std::string const & reqTarget, std::string const & originReqTarget, std::string const & root);
		void		transformHeader(stringMap const & header);
		void		buildEnvp();
		void    	execCgi();
	
		void		setCgiExecPath(std::string const & path);
		void		setReqMethod(std::string const & method);
		void		setContentLength(size_t const & length);
		void		setContentType(std::string const & type);

		std::string	getScriptFileName() const;

		void		clearCgiClass();
		
		void		displayCgiClass() const;

		static bool	checkIfFileCgi(std::string const & path);
};

#endif