/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   searchingUtils.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:30:53 by barandri          #+#    #+#             */
/*   Updated: 2025/10/31 09:20:42 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

vectServerIt	findFdInServer(vectServer & allServer, int const & fd)
{
	vectServerIt it;

	for(it = allServer.begin(); it != allServer.end(); it++)
		if ((*it).get_fdSocket() == fd)
			break;
	return it;
}

VClientIt	findFdInClient(VClient & allClient, int const & fd)
{
	VClientIt it;

	for(it = allClient.begin(); it != allClient.end(); it++)
	{
		if ((*it).get_clientFD() == fd)
			break;
	}
	if (it == allClient.end())
		throw std::runtime_error("Client not found");
	return it;
}


