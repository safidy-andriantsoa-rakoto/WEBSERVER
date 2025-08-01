/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:03:49 by barandri          #+#    #+#             */
/*   Updated: 2025/08/01 09:35:48 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	generateErrorResponse(int & fd, int const & statusCode)
{
    Response    response;
    std::string responseString;

    response.setType("text/html");
	responseString = response.responseErrorGenerator(statusCode, "./errorPage/error.html");
    send_value(fd, responseString);
}