/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 08:26:09 by barandri          #+#    #+#             */
/*   Updated: 2025/10/15 10:40:45 by barandri         ###   ########.fr       */
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
    PHP_SCRIPT
};

#endif

