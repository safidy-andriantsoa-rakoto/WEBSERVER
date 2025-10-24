/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorExcept.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:50:48 by barandri          #+#    #+#             */
/*   Updated: 2025/10/24 13:21:52 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorExcept.hpp"

ErrorExcept::ErrorExcept(int const & status, std::string const &errorMsg)
{
    _type = ERR_RSP;
    _statusCode = status;
    _errMsg = errorMsg;
}

ErrorExcept::ErrorExcept(int const &status, std::string const &body, std::string const &errorMsg)
{
    _type = ERR_RSP;
    _statusCode = status;
    _body = body;
    _errMsg = errorMsg; 
}

ErrorExcept::~ErrorExcept() throw(){}