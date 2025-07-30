/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:33:48 by barandri          #+#    #+#             */
/*   Updated: 2025/07/30 16:02:59 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GENERAL_HPP
# define GENERAL_HPP

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
#include <fcntl.h>
#include <csignal>
#include <cstdlib>
#include <csignal>
#include <typeinfo>
#include <errno.h>
#include <dirent.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>

#include "typedef.hpp"
#include "define.hpp"

//CGI
void        execCgi(std::string const & path, char **envp, int & pid, int & status, int & fd);
int         callCgi(char const **argv, char **envp);

//Utils
void        checkFileExtension(std::string const & fileName, std::string extension);
void	    printFileContent(std::string const & path);
std::string	readFileToString(std::string _path);
stringMap	splitFileToMapString(std::string const & filePath, std::string const & sep);
std::string	getFileExtension(std::string const & fileName);

void	    displayFormatting(std::string const & text, std::string const & title);
stringVect	strictSplit(std::string const & target, std::string const & sep);
void	    split_line(std::vector<std::string> &_data, std::string line);
void	    displayStringPair(stringPair const & pair);
void	    displayStringMap(stringMap const & map);
stringVect	splitString(std::string const & target, std::string const & sep);
std::string convertIntToString(int const & n);
void	    addToMapString(stringMap & map, std::string const & first, std::string const & second);
stringMapIt	findLongestMatch(std::string const & text, stringMap & map);

void        send_value(int &_acceptStatus, std::string &_send_content);
std::string response_generator(std::string response, std::string type, std::string content);

#endif