/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:11:42 by barandri          #+#    #+#             */
/*   Updated: 2025/10/31 10:17:28 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIPARSER_HPP
# define CGIPARSER_HPP

#include "general.hpp"

class CgiParser
{
	private:
		std::string _reqMethod;
		std::string _query;
		std::string _scriptFileName;
		std::string _scriptName;
		std::string	_contentType;
		std::string	_execPath;
		size_t		_contentLength;

		char**		_envp;

	public:
		CgiParser();
		CgiParser(CgiParser const & other);
		CgiParser& operator=(CgiParser const & other);
		~CgiParser();

		char**	getEnvp();

		void    parseCgiRequestTarget(std::string const & requestTarget);
		void	buildEnvp();
		void    execCgi();

		
		void	setCgiExecPath(std::string const & path);
		void	setReqMethod(std::string const & method);
		void	setContentLength(size_t const & length);
		void	setContentType(std::string const & type);

		std::string	getScriptFileName() const;

		void	clearCgiClass();
		
		void	displayCgiClass() const;

		static bool	checkIfFileCgi(std::string const & path);
};

#endif