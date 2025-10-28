/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedef.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:05 by barandri          #+#    #+#             */
/*   Updated: 2025/10/28 09:46:19 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEF_HPP
# define TYPEDEF_HPP

#include <utility>
#include <vector>
#include <map>
#include <list>
#include <string>
#include "general.hpp"

//Put general typedef here

typedef std::map<std::string, std::string >    MapMime;
typedef std::map<std::string, std::string>     stringMap;
typedef stringMap::const_iterator			   stringMapIt;

typedef std::pair<std::string, std::string>	   stringPair;

typedef std::vector<std::string>	stringVect;
typedef stringVect::iterator        stringVectIt;

typedef std::vector<int>		intVect;
typedef intVect::const_iterator	intVectIt;

typedef std::list<std::string>	stringList;
typedef std::list<int>			intList;

typedef struct sockaddr_in		SockAddrIn;
typedef struct epoll_event		SEpollEvent;

typedef struct s_errPage        t_errPage;
typedef std::vector<t_errPage>  errVect;
typedef errVect::iterator       errIt;

typedef std::map<stringVect, std::string>   MapSVectStr;
typedef MapSVectStr::iterator               MapSVectStrIt;

typedef struct s_redirection      t_redirection;

#endif