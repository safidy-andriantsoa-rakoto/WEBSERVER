/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 10:33:08 by barandri          #+#    #+#             */
/*   Updated: 2025/09/29 11:22:43 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::Cgi(){}

Cgi::~Cgi(){}

char **Cgi::getEnvp()
{
	return _envp;
}

void Cgi::parseCgiEnv(std::string const & requestTarget)
{
}

void Cgi::configEnvp()
{
//     "GATEWAY_INTERFACE=CGI/1.1",
//     "REQUEST_METHOD=GET",
//     "QUERY_STRING=username=Alice",
//     "SCRIPT_FILENAME=/var/www/html/welcome.php",
//     "SCRIPT_NAME=/welcome.php",
//     "SERVER_PROTOCOL=HTTP/1.1",
//     "CONTENT_LENGTH=0",
//     "REDIRECT_STATUS=200",
}

void Cgi::execCgi()
{
}
