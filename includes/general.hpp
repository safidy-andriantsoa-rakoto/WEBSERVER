/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:33:48 by barandri          #+#    #+#             */
/*   Updated: 2025/09/22 10:03:18 by trahanta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GENERAL_HPP
# define GENERAL_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <exception>
#include <exception>
#include <iomanip>
#include <sstream>
#include <algorithm>

#include <stdio.h>
#include <unistd.h>
#include <cmath>
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

// Error handling
void	generateErrorResponse(int & fd, int const & statusCode);

//CGI
void        execCgi(std::string const & path, char **envp, int & pid, int & status, int & fd);
int         callCgi(char const **argv, char **envp);

//Utils
    //fileUtils
void        checkFileExtension(std::string const & fileName, std::string extension);
void	    printFileContent(std::string const & path);
std::string	readFileToString(std::string _path);
stringMap	splitFileToMapString(std::string const & filePath, std::string const & sep);
std::string	getFileExtension(std::string const & fileName);

    //stringUtils
stringVect	strictSplit(std::string const & target, std::string const & sep);
void	    split_line(std::vector<std::string> &_data, std::string line);
stringVect	splitString(std::string const & target, std::string const & sep);
void	    addToMapString(stringMap & map, std::string const & first, std::string const & second);
void	    replaceString(std::string & text, std::string const & replaceText, std::string const & findText);
std::string	checkLimitSize(std::string const & a, std::string const & b);

    //Conversion
std::string convertIntToString(ssize_t const & n);
float       convertStringToFLoat(std::string const & n);
int         convertStringToInt(std::string const & n);
std::string	convertStringMapToString(stringMap const & map, std::string const & sep);

    //DisplayUtils
void	    displayFormatting(std::string const & text, std::string const & title);
void	    displayStringPair(stringPair const & pair);
void	    displayStringMap(stringMap const & map);
void	    displayString(std::string const & text);
void	    displayStringVect(stringVect const & vect, std::string const & title);
void	    displayStringVect(stringVect const & vect);

    //messageUtils
void        send_value(int const & _acceptStatus, std::string & _send_content);

//parse
bool        checkBraceClose(std::vector<std::string> vect);
bool        checkBracesAfterNewline(const std::vector<std::string> &file);
bool        checkCountWord(const std::vector<std::string> &lines, const std::string &word, const int &count);
bool        checkWriteConfig(const std::vector<std::string>& lines, const std::string &word, const std::string &writeLine);
bool        checkNumberAllContent(std::vector<std::string> &file);
std::vector<int> containsExactWordInVector(const std::vector<std::string> &lines, const std::string &word);

std::vector<std::string> getContentFile(std::string _path);


#endif