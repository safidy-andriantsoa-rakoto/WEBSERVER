/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 08:26:09 by barandri          #+#    #+#             */
/*   Updated: 2025/10/13 08:10:22 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUM_HPP
# define ENUM_HPP

enum   e_resp
{
    STD_RSP,
    REDIR_RSP,
    ERR_RSP,
    LDIR_RSP,
    CGI_RSP,
    SINGLE_RSP
};

enum    e_operation
{
    OP_ONCE,
    OP_OPEN,
    OP_START,
    OP_ONGOING,
    OP_CLOSE
};

enum    e_identifier
{
    SERVER,
    CLIENT,
    FILE_FD,
    CGI_FD,
    CGI_ERR_FD
};

enum    e_fdCategory
{
    FD_STDOUT,
    FD_STDERR
};

enum    e_fdRspType
{
    HTML_FILE,
    PHP_SCRIPT
};

#endif

