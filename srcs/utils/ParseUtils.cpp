/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 09:16:15 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/11/07 09:47:23 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

vectServer getServer(stringVect file)
{
    vectServer tabServer;
    Server server;
    stringVect vect;

    	for (size_t i = 0; i < file.size(); ++i)
        {
            // std::cout << "GET SERVER " << i << std::endl;
            std::string word;
            std::istringstream iss(file[i]);
            while(iss >> word)
            {
                if(word == "server")
                {
                    server = Server();
                    tabServer.push_back(server);
                }
            }
        }
    return (tabServer);
}

bool checkValuePort(intVect &vect)
{
    for(size_t i = 0; i < vect.size(); i++)
    {
        if(vect[i] < 0 || vect[i] > 65535)
            return (false);
    }
    return (true);
}

bool checkDuplicationStr(stringVect &str)
{
    size_t i(0), j(0);
    while(i < str.size())
    {
        j = i + 1;
        while(j < str.size())
        {
            if(str[i] == str[j])
                return (false);
            j++;
        }
        i++;
    }
    return (true);

}

bool checkDuplicationInt(intVect &Int)
{
    size_t i(0), j(0);
    while(i < Int.size())
    {
        j = i + 1;
        while(i < Int.size())
        {
            if(Int[i] == Int[j])
                return (false);
            j++;
        }
        i++;
    }
    return (true);
}

/* [!] : Keep function */
bool checkValueMethod(stringVect &str)
{
    for(size_t i = 0; i < str.size(); i++)
    {
        if(str[i] != "GET" && str[i] != "POST" && str[i] != "DELETE")
            return (false);
    }
    return (true);
}

bool    checkDuplicateInStringVect(stringVect & vect)
{
    for (stringVectIt it = vect.begin(); it != vect.end(); it++)
    {
        if (std::count(vect.begin(), vect.end(), *it) != 1)
            return true;
    }
    return false;
}

bool    findEltInStringVect(stringVect & vect, stringVect & toFind)
{
    for (stringVectIt it = toFind.begin(); it != toFind.end(); it++)
    {
        if (std::count(vect.begin(), vect.end(), *it) != 1)
            return false;
    }
    return true; 
}

s_dataConf  addDataConf(int const & lvl, std::string const & key, stringVect & value)
{
    s_dataConf conf;

    conf.lvl = lvl;
    conf.key = key;
    conf.value = value;
    return conf;
}

