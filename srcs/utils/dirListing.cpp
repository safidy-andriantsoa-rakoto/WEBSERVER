/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dirListing.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:25:59 by barandri          #+#    #+#             */
/*   Updated: 2025/09/23 15:21:26 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "general.hpp"

stringMap	listingDirectory(DIR* dir, std::string const & path)
{
	struct dirent*	tmp;
	stringMap		map;

	while ((tmp = readdir(dir)))
		map.insert(std::pair<std::string, std::string>(path + tmp->d_name, tmp->d_name));
	closedir(dir);
	return map;
}

std::string	convertListing(stringMap & mapDir)
{
	std::string	list;

	for(stringMapIt it = mapDir.begin(); it != mapDir.end(); it++)
		list += "		<li><a href=\"" + (*it).first + ">" + (*it).second + "</a></li>\n";
	return list;
}