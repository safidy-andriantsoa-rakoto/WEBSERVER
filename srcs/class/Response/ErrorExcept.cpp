/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorExcept.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:50:48 by barandri          #+#    #+#             */
/*   Updated: 2025/11/17 14:12:30 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorExcept.hpp"

ErrorExcept::ErrorExcept(int const & status, std::string const &errorMsg)
{
    _type = ERR_RSP;
    _statusCode = status;
    _errMsg = errorMsg;
}

ErrorExcept::~ErrorExcept() throw(){}