#ifndef CLASSUTILS_HPP
# define CLASSUTILS_HPP

#include "typedef.hpp"

#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iterator>

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

void	    displayStringPair(stringPair const & pair);
void	    displayStringMap(stringMap const & map);
stringVect	splitString(std::string const & target, std::string const & sep);
std::string	readFileToString(std::string _path);

std::string get_index_content(std::string _path);
std::string response_generator(std::string response, std::string type, std::string content);
void send_value(int &_acceptStatus, std::string &_send_content);

#endif