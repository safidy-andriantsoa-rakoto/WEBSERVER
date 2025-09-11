/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 09:16:15 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/09/11 08:06:18 by trahanta         ###   ########.fr       */
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

