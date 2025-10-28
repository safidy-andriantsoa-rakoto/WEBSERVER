/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 09:16:15 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/10/24 15:23:45 by barandri         ###   ########.fr       */
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

