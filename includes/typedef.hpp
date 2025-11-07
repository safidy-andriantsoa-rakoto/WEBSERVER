/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedef.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:05 by barandri          #+#    #+#             */
/*   Updated: 2025/11/07 20:49:47 by barandri         ###   ########.fr       */
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

typedef struct s_redirection	t_redirection;

typedef std::map<stringVect, std::string>   MapSVectStr;
typedef MapSVectStr::iterator               MapSVectStrIt;

struct s_dataConf
{
	int lvl;
	std::string key;
	stringVect  value;
};

typedef std::vector<s_dataConf>				vectData;
typedef std::vector<s_dataConf>::iterator	vectDataIt;

typedef std::vector<vectData>	serverVect;
typedef serverVect::iterator	serverVectIt;


#endif