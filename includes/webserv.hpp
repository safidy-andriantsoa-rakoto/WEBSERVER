/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:33:48 by barandri          #+#    #+#             */
/*   Updated: 2025/07/28 10:42:57 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <exception>
#include <iomanip>
#include <sstream>
#include <algorithm>

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <csignal>
#include <cstdlib>
#include <dirent.h>
#include <typeinfo>

#include "typedef.hpp"
#include "define.hpp"

#include "../srcs/class/Server.hpp"
#include "../srcs/class/Request.hpp"
#include "../srcs/class/Response.hpp"

typedef std::list<Server>   ServerList;

//CGI
void        execCgi(std::string const & path, char **envp, int & pid, int & status, int & fd);
int         callCgi(char const **argv, char **envp);

void	    printFileContent(std::string const & path);
void        checkExtension(std::string const & fileName, std::string extension);

//Safidy Functions
void        send_value(int &_acceptStatus, std::string &_send_content);
std::string	response_generator(std::string content, std::string type, size_t size);
void        get_request(Request & request, int &_acceptStatus);
void	    launchServer();

//Parsing request
void        parseRequest(std::string const & requestString, Request & request);

//Utils
void	    displayFormatting(std::string const & text, std::string const & title);
stringVect	strictSplit(std::string const & target, std::string const & sep);
void	    split_line(std::vector<std::string> &_data, std::string line);

std::string response_generator(std::string response, std::string type, std::string content);

#endif