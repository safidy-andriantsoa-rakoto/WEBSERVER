/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:28:23 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/07/28 09:49:01 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

void get_request(std::string _client_get, int &_acceptStatus, std::string path)
{
    std::string ext_file;
    std::vector<std::string> line;
    std::string f_line;
    std::string tmp_ext;
    std::string content;
    std::string response;

    size_t pos = _client_get.find_first_of("\n");
    f_line = _client_get.substr(0, pos - 1);
    std::cout << f_line << std::endl;

    split_line(line, f_line);
    size_t ps = line[1].find("?");
    line[1] = line[1].substr(0, ps);
    tmp_ext = line[1];
    int pe = tmp_ext.find_last_of(".");
    ext_file = tmp_ext.erase(0, pe + 1);
    // std::string path = "dim/"; //path makany @ilay site
    line[1].erase(0,1);
    if (line[0] == "GET")
    {
        if (ext_file == "/")
        {
            content = get_index_content(path + "index.html");
            response = response_generator("HTTP/1.1 200 OK\r\n",content, "text/html", content.size());// line request + content index.html
            // std::cout << "\n" << response << std::endl;
            send_value(_acceptStatus, response);
        }
        else if (ext_file == "css" || ext_file == "html" || ext_file == "plain")
        {
            // std::cout << "aty ee" << std::endl;
            path += line[1];
            content = get_index_content(path);
            std::string type = "text/";
            type += ext_file;
            response = response_generator("HTTP/1.1 200 OK\r\n",content, type, content.size());
            // std::cout << response << std::endl;
            send_value(_acceptStatus, response);
        }
        else if (ext_file == "js")
        {
            path += line[1];
            content = get_index_content(path);
            response = response_generator("HTTP/1.1 200 OK\r\n", content, "text/javascript", content.size());
            send_value(_acceptStatus, response);
        }
        else if (ext_file == "jpg" || ext_file == "png" || ext_file == "jpeg" || ext_file == "ico" || ext_file == "svg")
        {
            path += line[1];
            content = get_index_content(path);
            std::string ext = "image/";
            if (ext_file == "svg")
                ext += "svg+xml";  
            else
                ext += ext_file;
            response = response_generator("HTTP/1.1 200 OK\r\n", content, ext, content.size());
            send_value(_acceptStatus, response);
        }
        else if (ext_file == "pdf")
        {
            path += line[1];
            content = get_index_content(path);
            std::string ext = "application/";
            ext += ext_file;
            response = response_generator("HTTP/1.1 200 OK\r\n", content, ext, content.size());
            send_value(_acceptStatus, response);
        }
        else if (ext_file == "mp4")
        {
            path += line[1];
            content = get_index_content(path);
            std::string ext = "video/";
            ext += ext_file;
            response = response_generator("HTTP/1.1 200 OK\r\n" ,content, ext, content.size());
            send_value(_acceptStatus, response);
        }
        else if (ext_file == "mp3" || ext_file == "ogg" || ext_file == "webm" || ext_file == "m4a" || ext_file == "waw")
        {
            path += line[1];
            content = get_index_content(path);
            std::string ext = "audio/";
            ext += ext_file;
            response = response_generator("HTTP/1.1 200 OK\r\n" ,content, ext, content.size());
            send_value(_acceptStatus, response);
        }
        else if (ext_file == "woff" || ext_file == "woff2")
        {
            path += line[1];
            content = get_index_content(path);
            std::string ext = "font/";
            ext += ext_file;
            response = response_generator("HTTP/1.1 200 OK\r\n" ,content, ext, content.size());
            send_value(_acceptStatus, response);
        }
    }
    else if (line[0] == "POST")
    {
        std::string response = "<html><head><title>Success</title></head><body><h1>SUCCESS!</h1></body></html>";
        // path += line[1];
        // content = get_index_content(path);
        // size_t pst = content.find("</body>");
        // std::cout << "pos=" << pst << std::endl;
        std::string ext = "text/";
        ext += ext_file;
        response = response_generator("HTTP/1.1 201 Created\r\n",response, "text/html",response.size());
        send_value(_acceptStatus, response);
    }
    else
    {
        std::string badRequest(BADREQUEST);
        send_value(_acceptStatus, badRequest);
    }
}

