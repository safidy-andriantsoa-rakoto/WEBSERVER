/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:11:42 by barandri          #+#    #+#             */
/*   Updated: 2025/09/29 11:22:46 by barandri         ###   ########.fr       */
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
		int			_contentLength;

		char*		_envp[];

	public:
		Cgi();
		~Cgi();

		char	**getEnvp();

		void    parseCgiEnv();
		void	configEnvp();
		void    execCgi();
};

#endif