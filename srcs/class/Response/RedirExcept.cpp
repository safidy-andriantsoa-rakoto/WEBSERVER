/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirExcept.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:50:48 by barandri          #+#    #+#             */
/*   Updated: 2025/08/27 08:45:20 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RedirExcept.hpp"

RedirExcept::RedirExcept(int const & status, std::string const & errMsg, std::string const & uri, std::string const & listDir) : _uri(uri), _listDir(listDir)
{
    _statusCode = status;
    _errMsg = errMsg;
}

RedirExcept::RedirExcept(int const &status, std::string const &errMsg, std::string const &uri) : _uri(uri)
{
    _statusCode = status;
    _errMsg = errMsg;
}

RedirExcept::~RedirExcept() throw(){}