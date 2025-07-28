#pragma once

#include <string>
#include <csignal>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/stat.h>
#include <typeinfo>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "Utils.hpp"

#ifndef _NB_CLIENT
#define _NB_CLIENT 10
#endif

#ifndef _BUFFER
#define _BUFFER 1024
#endif

// #define POST "POST "
#define HTTP_REQUEST "HTTP/1.1 200 OK\r\n"
#define CONTENT_TYPE "Content-Type: text/html\r\n"
#define CONTENT_TYPE_CSS "Content-Type: text/css\r\n"
#define CONTENT_LENGTH "Content-Length: "
#define _EMPTY_LINE "\n\r\n"
#define BADREQUEST "HTTP/1.1 400 Bad Request\n\r\n<html><head></head> <body><h1 background=\"#ff0000\" >ERROR 400 BAD REQUEST.</h1></body></html>"



class Server
{
    private:
        int     _fdSocket;
        int     _fdClient;
        size_t  _size_sockaddrin;
        struct  sockaddr_in _sockaddrIn;

    public:
        Server();
        ~Server();
        int get_fdClient();
        int creat_socket();
        void configure_sockaddrin(int port);
        int link_socket_adress();
        int wait_connexion_from_client();
        int accept_client_connexion();
        std::string get_client_request();
};
