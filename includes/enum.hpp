/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 08:26:09 by barandri          #+#    #+#             */
/*   Updated: 2025/10/20 09:24:43 by rsafidy-         ###   ########.fr       */
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
    CGI_FD_OUTPUT,
    CGI_FD_ERR,
    CGI_FD_INPUT
};

enum    e_fdCategory
{
    FD_STDOUT,
    FD_STDERR,
    FD_STDIN
};

enum    e_fdRspType
{
    STD_FILE,
    GET_PHP_SCRIPT,
    POST_PHP_SCRIPT
};

#endif

