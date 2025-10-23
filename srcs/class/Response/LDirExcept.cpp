/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LDirExcept.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:50:48 by barandri          #+#    #+#             */
/*   Updated: 2025/09/15 14:54:01 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LDirExcept.hpp"

stringVect  LDirExcept::_part;

LDirExcept::LDirExcept(int const &status, DIR *dir, std::string const & path, std::string const & errMsg)
{
    _type = LDIR_RSP;
    _statusCode = status;
    _errMsg = errMsg;
    _dirList = listingDirectory(dir, path);
}

LDirExcept::~LDirExcept() throw() {}

void LDirExcept::initPart()
{
    _part.push_back("<!DOCTYPE html></head><body><h1>LISTING DIRECTORY</h1><div><ul>");
    _part.push_back("</ul></div></body></html>");
}
