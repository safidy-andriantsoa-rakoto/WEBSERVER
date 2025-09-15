/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirExcept.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:50:48 by barandri          #+#    #+#             */
/*   Updated: 2025/09/15 14:54:01 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DirExcept.hpp"

DirExcept::DirExcept(int const &status, DIR *dir, std::string const & path, std::string const & errMsg)
{
    _statusCode = status;
    _errMsg = errMsg;
    _dirTree = listingDirectory(dir, path);
}

DirExcept::~DirExcept() throw() {}