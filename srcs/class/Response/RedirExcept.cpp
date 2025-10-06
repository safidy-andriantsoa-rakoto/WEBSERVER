/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirExcept.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:50:48 by barandri          #+#    #+#             */
/*   Updated: 2025/09/19 08:32:00 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RedirExcept.hpp"

RedirExcept::RedirExcept(int const & status, std::string const & errMsg, std::string const & uri, std::string const & listDir) : _uri(uri), _upperDir(listDir)
{
    _type = REDIR_RSP;
    _statusCode = status;
    _errMsg = errMsg;
}

RedirExcept::RedirExcept(int const &status, std::string const &errMsg, std::string const &uri) : _uri(uri)
{
    _type = REDIR_RSP;
    _statusCode = status;
    _errMsg = errMsg;
}

RedirExcept::~RedirExcept() throw(){}