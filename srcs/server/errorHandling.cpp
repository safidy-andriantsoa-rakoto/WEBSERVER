/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:03:49 by barandri          #+#    #+#             */
/*   Updated: 2025/07/30 09:30:27 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	generateErrorResponse(int & fd, int const & statusCode)
{
    Response    response;
    std::string responseString;

    // std::cout << "Generate error\n";
	responseString = response.responseGenerator(statusCode, "site/error.html", "text/html");
    send_value(fd, responseString);
}