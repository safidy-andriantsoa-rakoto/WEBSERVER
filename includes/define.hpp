/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:40:59 by barandri          #+#    #+#             */
/*   Updated: 2025/10/06 09:31:03 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_HPP
# define DEFINE_HPP

//CGI
#define FILE_EXT ".php"
#define RESPONSE_FILE_NAME "response.txt"

//PATH
#define ERR_PATH "./config/errorPage/default/index.html"
#define STAT_PATH "./config/statcode/statusCode.conf"
#define LDIR_PATH "./config/listingDirectory/listDir.html"
#define MIME_PATH "./config/ext"
#define CGI_PATH  "./config/cgiConf/cgi.conf"
#define PHP_CGI_PATH "/usr/bin/php-cgi"

#define DEF_ERR_LOCATION "location /errorPage | root ./config/errorPage/default | method GET POST DELETE"

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