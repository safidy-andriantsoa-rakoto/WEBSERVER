/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:11:42 by barandri          #+#    #+#             */
/*   Updated: 2025/09/30 10:57:02 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include "general.hpp"

class Cgi
{
	private:
		std::string _reqMethod;
		std::string _query;
		std::string _scriptFileName;
		std::string _scriptName;
		size_t		_contentLength;

		char**		_envp;

		int			_cgiFd;
		int			_clientFd;

	public:
		Cgi();
		Cgi(Cgi const & other);
		Cgi& operator=(Cgi const & other);
		~Cgi();

		char**	getEnvp();

		void    parseCgiEnv(std::string const & requestTarget, std::string const & rootDirectory);
		void	configEnvp();
		void    execCgi();

		void	setReqMethod(std::string const & method);
		void	setContentLength(size_t const & length);

		void	displayCgiClass() const;

		static stringMap	_cgiConfig;
		static void			initCgiConfig(std::string const & path);
};

#endif