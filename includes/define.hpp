/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:40:59 by barandri          #+#    #+#             */
/*   Updated: 2025/11/11 20:58:52 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_HPP
# define DEFINE_HPP

//PATH
#define ERR_PATH "./config/errorPage/default/default.html"
#define MIME_PATH "./config/ext"

//HTTP
#define HTTP_REQUEST "HTTP/1.1 200 OK\r\n"
#define CONTENT_TYPE "Content-Type: text/html\r\n"
#define CONTENT_TYPE_CSS "Content-Type: text/css\r\n"
#define CONTENT_LENGTH "Content-Length: "
#define CRLF_SEP "\r\n\r\n"
#define _NB_CLIENT 10
#define MAX_EVENT 2042
#define _BUFFER 1000
#define _BUFFER_RECV 1024
#define	CLIENT_MAX_TIME 60
#define	CGI_MAX_TIME CLIENT_MAX_TIME / 2
#define	HEADER_MAX_SIZE 2000

//Listing directory
#define HEAD_LDIR "<!DOCTYPE html></head><body><h1>LISTING DIRECTORY</h1><div><ul>"
#define TAIL_LDIR "</ul></div></body></html>"

//Status Code
#define ALL_STATUS_CODE "200;201;204;301;302;307;308;400;403;404;405;408;411;413;414;415;421;500;501;502;503;504;505"

#define BACKUP_ERR_PAGE "<!DOCTYPE html><html><head>" \
                        "<meta charset=\"UTF-8\">" \
                        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">" \
                        "<title>Default error page</title>" \
                        "<style>body {min-height: 100vh;margin: 0;font-family: sans-serif;background-color: white;color: black;display: flex;justify-content: center;align-items: center;}" \
                        ".content {padding: 40px;background-color: #f0f0f0;border: 1px solid black;max-width: 90%;text-align: center;}" \
                        "h1 {margin-bottom: 10px;}</style>" \
                        "</head><body><div class=\"content\"><h1>ERROR</h1><h2>$status</h2></div></body></html>"

#define DEFAULT_CONFIG "server\n" \
						"	host localhost\n" \
						"	port 2003\n" \
						"	size 100 MB\n" \
						"	location /\n" \
						"		root ./site\n" \
						"		method GET POST DELETE\n" \
						"		autoindex ON\n" \
						"		cgi	ON /usr/bin/php-cgi"

#define DEFAULT_UPLOAD_LOCATION "	location /upload\n" \
								"		root $1\n" \
								"		method GET POST DELETE\n" \
								"		autoindex ON"

#endif