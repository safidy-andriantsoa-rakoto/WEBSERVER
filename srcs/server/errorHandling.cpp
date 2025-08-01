/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:03:49 by barandri          #+#    #+#             */
/*   Updated: 2025/07/31 11:29:11 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	generateErrorResponse(int & fd, int const & statusCode)
{
    Response    response;
    std::string responseString;

    std::cout << "RESPONSE ERROR SENT!!!!" << std::endl;
	responseString = response.responseGenerator(statusCode, "./errorPage/error.html", "text/html");
    send_value(fd, responseString);
}