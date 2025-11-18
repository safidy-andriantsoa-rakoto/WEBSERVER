/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:40:59 by barandri          #+#    #+#             */
/*   Updated: 2025/11/18 08:10:45 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_HPP
# define DEFINE_HPP

//PATH
#define ERR_PATH "./config/errorPage/default/default.html"
#define MIME_PATH "./config/ext"

//HTTP
#define CRLF_SEP "\r\n\r\n"
#define MAX_EVENT 2042
#define _BUFFER 1000
#define _BUFFER_RECV 1024
#define	CLIENT_MAX_TIME 60
#define	CGI_MAX_TIME CLIENT_MAX_TIME / 2
#define LIM_PORT 3

//Listing directory
#define HEAD_LDIR "<!DOCTYPE html></head><body><h1>LISTING DIRECTORY</h1><div><ul>"
#define TAIL_LDIR "</ul></div></body></html>"

//Status Code
#define ALL_STATUS_CODE "200;201;204;301;302;307;308;400;403;404;405;408;411;413;500;501;502;504;505"

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
						"	size 250 MB\n" \
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