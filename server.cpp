/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/07/14 10:52:35 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string get_index_content(std::string _path)
{
    std::fstream index;
    index.open(_path.c_str());
    if (index.is_open())
    {
        std::string _index_value((std::istreambuf_iterator<char>(index)), std::istreambuf_iterator<char>());
        return (_index_value);
    }
    return ("HTTP/1.1 404 Not Found\n\r\n");//tsita zoky
}

void send_value(int &_acceptStatus, std::string &_send_content)
{
    size_t tb = 0;
    while (tb < _send_content.size())
    {
        size_t _send_request = send(_acceptStatus, _send_content.c_str(), _send_content.size(), 0);
        if (_send_request < 0)
            std::cout << "Error\nSend" << std::endl;
        tb += _send_request;
    }
}

void split_line(std::vector<std::string> &_data, std::string line)
{
    size_t pos = -1;
    pos =  line.find_first_of(" ");
    while (pos != size_t(-1))
    {
        _data.push_back(line.substr(0, pos));
        line.erase(0, pos + 1);
        pos =  line.find_first_of(" ");
    }
    if (line.size())
    _data.push_back(line);
}

std::string response_generator(std::string content, std::string type, size_t size)
{
    std::stringstream out;
    out << "HTTP/1.1 200 OK\r\n";// misy ilay zavatra nangatahin'ilay client
    out << "Content-Type: " << type << "\r\nAccess-Control-Allow-Origin: *\r\n";// mime text/html
    out << "Content-Length: " << size << "\n\r\n";// index.html -> string -> omena ny size 
    out << content << "\r\n";
    return (out.str());
}

void get_request(std::string _client_get, int &_acceptStatus)
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
    for (size_t i = 0; i < line.size(); i++)
        std::cout << "Request:" << line[i] << std::endl;
    if (line[0] == "GET")
    {
        tmp_ext = line[1];
        int pe = tmp_ext.find_last_of(".");
        std::cout << "Find . pos:" << pe << std::endl;
        ext_file = tmp_ext.erase(0, pe + 1);
        std::cout << "extension->" << ext_file << std::endl;
        std::string path = "site/";
        line[1].erase(0,1);
        if (ext_file == "/")
        {
            content = get_index_content("index.html");
            response = response_generator(content, "text/html", content.size());// line request + content index.html
            // std::cout << "\n" << response << std::endl;
            send_value(_acceptStatus, response);
        }
        else if (ext_file == "css" || ext_file == "html" || ext_file == "plain")
        {
            std::cout << "aty ee" << std::endl;
            path += line[1];
            content = get_index_content(path);
            std::string type = "text/";
            type += ext_file;
            response = response_generator(content, type, content.size());
            send_value(_acceptStatus, response);
        }
        else if (ext_file == "js")
        {
            path += line[1];
            content = get_index_content(path);
            response = response_generator(content, "text/javascript", content.size());
            send_value(_acceptStatus, response);
        }
        else if (ext_file == "jpg" || ext_file == "png" || ext_file == "jpeg" || ext_file == "ico")
        {
            path += line[1];
            content = get_index_content(path);
            std::string ext = "image/";
            ext += ext_file;
            response = response_generator(content, ext, content.size());
            send_value(_acceptStatus, response);
        }
        else if (ext_file == "woff" || ext_file == "pdf")
        {
            path += line[1];
            content = get_index_content(path);
            std::string ext = "application/";
            ext += ext_file;
            response = response_generator(content, ext, content.size());
            send_value(_acceptStatus, response);
        }
        else if (ext_file == "mp4")
        {
            path += line[1];
            content = get_index_content(path);
            std::string ext = "video/";
            ext += ext_file;
            response = response_generator(content, ext, content.size());
            send_value(_acceptStatus, response);
        }
    }
}

int main()
{
    int _socketFd = socket(AF_INET, SOCK_STREAM, 0);

    if (_socketFd == -1)
        std::cout << "Error\ncreate an endpoint for communication" << std::endl;
    struct sockaddr_in _sockaddrIn;
    _sockaddrIn.sin_family = AF_INET;
    _sockaddrIn.sin_port = htons(2003);
    _sockaddrIn.sin_addr.s_addr = INADDR_ANY;//ip all

    int size = sizeof(_sockaddrIn);
    int _bindStatus = bind(_socketFd, (sockaddr *)&_sockaddrIn, size);
    if (_bindStatus == -1)
        std::cout << "Error\nbind a name to a socket" << std::endl;

    int _listenStatus = listen(_socketFd, 5); // client 1 ihany
    if (_listenStatus == -1)
        std::cout << "Error\nlisten for connections on a socket" << std::endl;
    std::cout << "Wait..." << std::endl;

    while (1)
    {
        int _acceptStatus = accept(_socketFd, (sockaddr *)&_sockaddrIn, (socklen_t *)&size);
        if (_acceptStatus == -1)
            std::cout << "Error\naccept a connection on a socket" << std::endl;
        else
            std::cout << "Connected" << std::endl;
            
            
        char resp[1024] = {0};
        int _respStatus = recv(_acceptStatus, resp, sizeof(resp), 0);
        if (_respStatus == -1)
            std::cout << "Error\nreceive a message from a socket" << std::endl;
        std::string get = resp;
        std::cout << get <<  std::endl;
        get_request(get, _acceptStatus);
        close(_acceptStatus);
    }
    close(_socketFd);
    return 0;
}
