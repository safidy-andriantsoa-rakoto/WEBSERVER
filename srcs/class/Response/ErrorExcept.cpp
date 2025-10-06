/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorExcept.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:50:48 by barandri          #+#    #+#             */
/*   Updated: 2025/09/19 08:32:16 by barandri         ###   ########.fr       */
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