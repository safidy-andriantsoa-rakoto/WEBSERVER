/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:40:59 by barandri          #+#    #+#             */
/*   Updated: 2025/10/20 14:07:47 by trahanta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_HPP
# define DEFINE_HPP

//CGI
#define FILE_EXT ".php"
#define RESPONSE_FILE_NAME "response.txt"

//PATH
#define ERR_PATH "./config/errorPage/default/default.html"
#define STAT_PATH "./config/statcode/statusCode.conf"
#define LDIR_PATH "./config/listingDirectory/listDir.html"
#define MIME_PATH "./config/ext"
#define CGI_PATH  "./config/cgiConf/cgi.conf"
#define CGI_ERR_PATH  "./config/cgiConf/errCgi.conf"
#define PHP_CGI_PATH "/usr/bin/php-cgi"
#define UPLOAD_PATH "./upload"

#define DEF_UPLOAD_LOCATION "location /upload | root ./upload | method GET POST DELETE | autoindex ON"

//HTTP
#define HTTP_REQUEST "HTTP/1.1 200 OK\r\n"
#define CONTENT_TYPE "Content-Type: text/html\r\n"
#define CONTENT_TYPE_CSS "Content-Type: text/css\r\n"
#define CONTENT_LENGTH "Content-Length: "
#define _EMPTY_LINE "\n\r\n"
#define _NB_CLIENT 10
#define MAX_EVENT 2042
#define _BUFFER 1000 
#define _BUFFER_RECV 1024

#endif