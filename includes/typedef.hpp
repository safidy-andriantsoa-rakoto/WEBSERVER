#ifndef TYPEDEF_HPP
# define TYPEDEF_HPP

#include <utility>
#include <vector>
#include <map>
#include <list>
#include <string>

typedef std::map<std::string, std::string>  stringMap;
typedef stringMap::iterator					stringMapIt;

typedef std::pair<std::string, std::string>	stringPair;

typedef std::vector<std::string>	stringVect;
typedef std::vector<int>			intVect;

typedef std::list<std::string>	stringList;
typedef std::list<int>			intList;

#endif