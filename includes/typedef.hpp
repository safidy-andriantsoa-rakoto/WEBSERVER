/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedef.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:41:05 by barandri          #+#    #+#             */
/*   Updated: 2025/08/08 09:11:08 by rsafidy-         ###   ########.fr       */
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
typedef stringMap::const_iterator					stringMapIt;

typedef std::pair<std::string, std::string>	stringPair;

typedef std::vector<std::string>	stringVect;
typedef std::vector<int>			intVect;

typedef std::list<std::string>	stringList;
typedef std::list<int>			intList;
typedef struct sockaddr_in SockAddrIn;
typedef struct epoll_event SEpollEvent;

#endif