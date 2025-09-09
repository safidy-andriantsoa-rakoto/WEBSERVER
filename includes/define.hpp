/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:40:59 by barandri          #+#    #+#             */
/*   Updated: 2025/09/09 09:30:26 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_HPP
# define DEFINE_HPP

//CGI
#define FILE_EXT ".php"
#define RESPONSE_FILE_NAME "response.txt"

//Response
#define ERR_PATH "errorPage/default/index.html"
#define STAT_PATH "ext/statcode/statusCode.txt"

// #define POST "POST "
#define HTTP_REQUEST "HTTP/1.1 200 OK\r\n"
#define CONTENT_TYPE "Content-Type: text/html\r\n"
#define CONTENT_TYPE_CSS "Content-Type: text/css\r\n"
#define CONTENT_LENGTH "Content-Length: "
#define _EMPTY_LINE "\n\r\n"

#define MIME_PATH "ext"

#ifndef _NB_CLIENT
#define _NB_CLIENT 10
#endif

#ifndef _BUFFER
#define _BUFFER 1024
#endif

#define MAX_EVENT 2042

#endif