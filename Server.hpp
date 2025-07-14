#pragma once

#include <string>
#include <csignal>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/stat.h>
#include <typeinfo>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>

// #define POST "POST "
#define HTTP_REQUEST "HTTP/1.1 200 OK\r\n"
#define CONTENT_TYPE "Content-Type: text/html\r\n"
#define CONTENT_TYPE_CSS "Content-Type: text/css\r\n"
#define CONTENT_LENGTH "Content-Length: "
#define _EMPTY_LINE "\n\r\n"

