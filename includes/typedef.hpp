/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedef.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:05 by barandri          #+#    #+#             */
/*   Updated: 2025/11/06 14:28:11 by barandri         ###   ########.fr       */
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
typedef stringVect::const_iterator	stringVectCIt;

typedef std::vector<int>		intVect;
typedef intVect::const_iterator	intVectIt;

typedef std::vector<std::pair<int, int> >			intPairVect;
typedef std::vector<std::pair<int, int> >::iterator	intPairVectIt;

typedef std::list<std::string>	stringList;
typedef std::list<int>			intList;

typedef struct sockaddr_in		SockAddrIn;
typedef struct epoll_event		SEpollEvent;

typedef struct s_errPage        t_errPage;
typedef std::vector<t_errPage>  errVect;
typedef errVect::iterator       errIt;

typedef std::map<stringVect, std::string>   MapSVectStr;
typedef MapSVectStr::iterator               MapSVectStrIt;

/*stringVect = key, stringVect = Content (especially for location)*/
typedef std::map<stringVect, stringVect>	parseMap;
typedef parseMap::iterator					parseMapIt;
typedef parseMap::const_iterator			parseMapCit;

/*string = indentation, parseMap = content*/
typedef std::map<std::string, parseMap>     dataMap;
typedef dataMap::iterator                   dataMapIt;

typedef struct s_redirection      t_redirection;

#endif