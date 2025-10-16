/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 08:26:09 by barandri          #+#    #+#             */
/*   Updated: 2025/10/16 08:56:26 by barandri         ###   ########.fr       */
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
    FD_PRINCIPAL,
    FD_STDERR,
    FD_STDIN
};

enum    e_fdRspType
{
    HTML_FILE,
    GET_PHP_SCRIPT,
    POST_PHP_SCRIPT
};

#endif

