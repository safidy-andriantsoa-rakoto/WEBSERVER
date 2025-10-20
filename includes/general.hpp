/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:33:48 by barandri          #+#    #+#             */
/*   Updated: 2025/10/20 09:44:56 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <exception>
#include <exception>
#include <iomanip>
#include <sstream>
#include <algorithm>

#include <cstdio>
#include <unistd.h>
#include <cmath>
#include <fcntl.h>
#include <csignal>
#include <cstdlib>
#include <csignal>
#include <typeinfo>
#include <cerrno>
#include <dirent.h>
#include <cstring>

#include <string.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>

#include "typedef.hpp"
#include "define.hpp"
#include "enum.hpp"
#include <sys/time.h>
#include <ctime>

//Utils
    //fileUtils
void        checkFileExtension(std::string const & fileName, std::string extension);
void	    printFileContent(std::string const & path);
std::string	readFileToString(std::string _path);
stringMap	splitFileToMapString(std::string const & filePath, std::string const & sep);
std::string	getFileExtension(std::string const & fileName);
int         openFile(std::string const & filePath);
size_t	    getFileSize(std::string const & path);
DIR*	    openDir(std::string const & dirPath);
DIR*	    openDir(std::string const & dirPath);

    //stringUtils
stringVect	strictSplit(std::string const & target, std::string const & sep);
void	    split_line(std::vector<std::string> &_data, std::string line);
stringVect	splitString(std::string const & target, std::string const & sep);
void	    addToMapString(stringMap & map, std::string const & first, std::string const & second);
void	    replaceString(std::string & text, std::string const & replaceText, std::string const & findText);
std::string	checkLimitSize(std::string const & a, std::string const & b);
void	    chunkingString(std::string & text, int const & status);
void	    clearCharTable(char **tab);

std::string	divideStringBySize(std::string const & target, size_t const & startPos, size_t const & bufferSize);

    //Conversion
std::string convertIntToString(ssize_t const & n);
float       convertStringToFLoat(std::string const & n);
int         convertStringToInt(std::string const & n);
std::string	convertStringMapToString(stringMap const & map, std::string const & sep);
char**	    convertStringVectToTable(stringVect & vect);

    //DisplayUtils
void	    displayFormatting(std::string const & text, std::string const & title);
void	    displayStringPair(stringPair const & pair);
void	    displayStringMap(stringMap const & map);
void	    displayString(std::string const & text);
void	    displayStringVect(stringVect const & vect, std::string const & title);
void	    displayStringVect(stringVect const & vect);
void	    displayCharTable(char	**tab, std::string const & title);

//Parse
bool        checkBraceClose(std::vector<std::string> vect);
bool        checkBracesAfterNewline(const std::vector<std::string> &file);
bool        checkCountWord(const std::vector<std::string> &lines, const std::string &word, const int &count);
bool        checkWriteConfig(const std::vector<std::string>& lines, const std::string &word, const std::string &writeLine);
bool        checkNumberAllContent(std::vector<std::string> &file);

std::vector<std::string> getContentFile(std::string _path);
std::vector<int> containsExactWordInVector(const std::vector<std::string> &lines, const std::string &word);

std::vector<std::string> getContentFile(std::string _path);
bool        checkDuplicate(stringVect file, std::string name, size_t &start , size_t &end);
bool        checkConfigDupli(stringVect file, size_t &start, size_t &end);

// dirListing
stringMap	listingDirectory(DIR* dir, std::string const & path);
std::string	convertListing(stringMap & mapDir);

//pipeUtils
void	makeDup2(int const & fd1, int const & fd2, int const & fd3);
void	makeDup2(int const & fd1, int const & fd2);
void    dupStandardIO(int *stdFd, int const & n);
