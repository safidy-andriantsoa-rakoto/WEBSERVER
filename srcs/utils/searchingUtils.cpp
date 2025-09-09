/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   searchingUtils.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:30:53 by barandri          #+#    #+#             */
/*   Updated: 2025/09/03 09:31:10 by barandri         ###   ########.fr       */
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

	for(it = allClient.begin(); it != allClient.end() && (*it).get_clientFD() != fd; it++);
	return it;
}
