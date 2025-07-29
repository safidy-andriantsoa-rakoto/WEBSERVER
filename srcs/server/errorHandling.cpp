/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:03:49 by barandri          #+#    #+#             */
/*   Updated: 2025/07/29 10:58:13 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	generateErrorResponse(int & fd, int const & statusCode, std::string const & reasonPhrase)
{
    Response    response;
    std::string responseString;

    response.setStatusLine(statusCode, reasonPhrase);
    response.addHeader("Content-Type", "text/html");
    response.addHeader("Content-Lenght", response.getResponseFile().size());
    response.addHeader("Access-Control-Allow-Origin", "*");
    response.setResponseBody("site/error.html");
	responseString = response.responseGenerator();
    send_value(fd, responseString);
}