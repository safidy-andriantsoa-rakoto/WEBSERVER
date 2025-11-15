/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:33:48 by barandri          #+#    #+#             */
/*   Updated: 2025/11/15 15:51:22 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <exception>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <typeinfo>

#include <cstdio>
#include <cmath>
#include <csignal>
#include <cstdlib>
#include <csignal>
#include <cerrno>
#include <cstring>
#include <ctime>

#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/epoll.h>

#include "typedef.hpp"
#include "define.hpp"
#include "enum.hpp"

			/*	UTILS */

//	fileUtils
bool        checkFileExtension(std::string const & fileName, std::string extension);
std::string	readFileToString(std::string _path);
std::string	readFromRegFileFd(int & fd, int & stat);
std::string	getFileExtension(std::string const & fileName);
stringVect	getContentFile(std::string const & _path);
int         openRegFile(std::string const & filePath);
size_t	    getFileSize(std::string const & path);
bool		checkIfDirectoryAvailable(std::string const & path);
bool		checkIfFileAvailabe(std::string const & path, int const & flag);
bool		checkIfParentDirectory(std::string const & parentPath, std::string const & childPath);

//stringUtils
stringVect	strictSplit(std::string const & target, std::string const & sep);
stringVect	splitString(std::string const & target, std::string const & sep);
void	    split_line(std::vector<std::string> &_data, std::string line);
void	    addToMapString(stringMap & map, std::string const & first, std::string const & second);
void	    replaceString(std::string & text, std::string const & replaceText, std::string const & findText);
char**	    convertStringVectToTable(stringVect & vect);
void	    clearCharTable(char ***tab);
std::string	divideStringBySize(std::string const & target, size_t const & startPos, size_t const & bufferSize);
intVect 	containsExactWordInVector(stringVect const & lines, const std::string & word);
bool 		checkDuplicate(stringVect const & file, std::string const & name, size_t & start, size_t & end);

//	Conversion
std::string convertIntToString(ssize_t const & n);
float       convertStringToFLoat(std::string const & n);
int         convertStringToInt(std::string const & n);
std::string	convertStringMapToString(stringMap const & map, std::string const & sep);
float		convertToBytes(stringVect const & vect);

//	DisplayUtils
void	    displayFormatting(std::string const & text, std::string const & title);
void	    displayStringPair(stringPair const & pair);
void	    displayStringMap(stringMap const & map);
void	    displayString(std::string const & text);
void	    displayStringVect(stringVect const & vect, std::string const & title);
void	    displayStringVect(stringVect const & vect);
void	    displayCharTable(char	**tab, std::string const & title);
void		displayServerVectConfig(serverVect & allServer_conf);
void 		print_conf(serverVect & allServer_conf);
void		displayInt(int const & n);
void		displayIntVect(intVect const & vect, std::string const & title);

// dirListing
stringMap	listingDirectory(DIR* dir, std::string const & path);
std::string	createListingDirectoryLink(stringMap & mapDir);

//	pipeUtils
void		makeDup2(int const & fd1, int const & fd2, int const & fd3);
void		makeDup2(int const & fd1, int const & fd2);
void   		dupStandardIO(int *stdFd);

//	ParseUtils
bool		checkValuePort(intVect &vect);
bool		checkDuplicationStr(stringVect &str);
bool		checkDuplicationInt(intVect &Int);
bool		checkValueMethod(stringVect &str);
bool    	findEltInStringVect(stringVect & vect, stringVect & toFind);
s_dataConf  addDataConf(int const & lvl, std::string const & key, stringVect & value);
