/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:40:59 by barandri          #+#    #+#             */
/*   Updated: 2025/11/07 19:48:36 by barandri         ###   ########.fr       */
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

//Listing directory
#define HEAD_LDIR "<!DOCTYPE html></head><body><h1>LISTING DIRECTORY</h1><div><ul>"
#define TAIL_LDIR "</ul></div></body></html>"

//Status Code
#define ALL_STATUS_CODE "200;201;204;301;302;307;308;400;403;404;405;406;408;409;411;412;413;414;415;417;421;500;501;502;503;504;505"

#define	CGI_ENVP_CONF "REQUEST_METHOD=$1\n" \
						"SCRIPT_FILENAME=$2\n" \
						"SCRIPT_NAME=$3\n" \
						"REQUEST_URI=$4\n" \
						"QUERY_STRING=$5\n" \
						"CONTENT_LENGTH=$6\n" \
						"CONTENT_TYPE=$7\n" \
						"GATEWAY_INTERFACE=CGI/1.1\n" \
						"SERVER_PROTOCOL=HTTP/1.1\n" \
						"REDIRECT_STATUS=200\n" \
						"REMOTE_ADDR=10.11.5.6\n"

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