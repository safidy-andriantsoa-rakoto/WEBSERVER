/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedef.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:05 by barandri          #+#    #+#             */
/*   Updated: 2025/08/11 17:59:05 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEF_HPP
# define TYPEDEF_HPP

#include <utility>
#include <vector>
#include <map>
#include <list>
#include <string>

typedef std::map<std::string, std::string>  stringMap;
typedef stringMap::const_iterator			stringMapIt;

typedef std::pair<std::string, std::string>	stringPair;

typedef std::vector<std::string>	stringVect;
typedef stringVect::iterator        stringVectIt;

typedef std::vector<int>			intVect;

typedef std::list<std::string>	stringList;
typedef std::list<int>			intList;

typedef struct sockaddr_in SockAddrIn;
typedef struct epoll_event SEpollEvent;

//Class for config
typedef struct s_errPage t_errPage;
typedef std::vector<t_errPage>  errVect;
typedef errVect::iterator       errIt;

enum e_location
{
    AUTOINDEX,
    LOCATION,
    ROOT,
    METHOD,
    REDIRECT,
    INDEX
};

#endif