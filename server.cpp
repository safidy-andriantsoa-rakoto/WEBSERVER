/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:56:47 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/07/28 09:50:58 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(){}

Server::~Server()
{
    if (this->_fdClient != -1)
        close(this->_fdClient);
    if (this->_fdSocket != -1)
        close(this->_fdSocket);
}

int Server::get_fdClient()
{
    return (this->_fdClient);
}

int Server::creat_socket()
{
    this->_fdSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_fdSocket == -1)
    {
        std::cout << "Error\ncreate an endpoint for communication" << std::endl;
        return (0); 
    }
    return (1);
}

void Server::configure_sockaddrin(int port)
{
    this->_sockaddrIn.sin_family = AF_INET;
    this->_sockaddrIn.sin_port = htons(port);
    this->_sockaddrIn.sin_addr.s_addr = INADDR_ANY;
    this->_size_sockaddrin = sizeof(_sockaddrIn);
}

int Server::link_socket_adress()
{
    int     status;

    status = bind(this->_fdSocket, (sockaddr *)&(this->_sockaddrIn), this->_size_sockaddrin);
    if (status == -1)
    {
        std::cout << "Error\nbind a name to a socket" << std::endl;
        return (0);
    }
    return (1);
}

int Server::wait_connexion_from_client()
{
    this->_fdClient = listen(this->_fdSocket, _NB_CLIENT);
    std::cout << "Wait..." << std::endl;
    if (this->_fdClient == -1)
    {
        std::cout << "Error\nlisten for connections on a socket" << std::endl;
        return (0);    
    }
    return (1);
}

int Server::accept_client_connexion()
{
    this->_fdClient = accept(_fdSocket, (sockaddr *)&(this->_sockaddrIn), (socklen_t *)&(this->_size_sockaddrin));
    if (this->_fdClient == -1)
    {
        std::cout << "Error\naccept a connection on a socket" << std::endl;
        return (0);
    }
    return (1);
}

std::string Server::get_client_request()
{
    char response[_BUFFER];
    int _respStatus = recv(this->_fdClient, response, sizeof(response), 0);
    if (_respStatus == -1)
    {
        std::cout << "Error\nreceive a message from a socket" << std::endl;
        return ("");
    }
    return (response);
}

/************************************************************/

std::string response_generator(std::string response,std::string content, std::string type, size_t size)
{
    std::stringstream out;
    out << response;// misy ilay zavatra nangatahin'ilay client
    out << "Content-Type: " << type << "\r\nAccess-Control-Allow-Origin: *\r\n";// mime text/html
    out << "Content-Length: " << size << "\n\r\n";// index.html -> string -> omena ny size 
    out << content << "\r\n";
    return (out.str());
}

std::string get_index_content(std::string _path)
{
    std::fstream index;
    index.open(_path.c_str());
    if (index.is_open())
    {
        std::string _index_value((std::istreambuf_iterator<char>(index)), std::istreambuf_iterator<char>());
        index.close();
        return (_index_value);
    }
    index.open("404.html");
    std::string _index_value((std::istreambuf_iterator<char>(index)), std::istreambuf_iterator<char>());
    index.close();
    return (_index_value);
}

void send_value(int &_acceptStatus, std::string &_send_content)
{
    size_t tb = 0;
    while (tb < _send_content.size())
    {
        size_t _send_request = send(_acceptStatus, _send_content.c_str(), _send_content.size(), 0);
        if (!_send_request)
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
