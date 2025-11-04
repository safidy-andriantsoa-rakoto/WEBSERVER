/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dirListing.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:25:59 by barandri          #+#    #+#             */
/*   Updated: 2025/11/04 14:09:32 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "general.hpp"

stringMap	listingDirectory(DIR* dir, std::string const & relativePath)
{
	struct dirent*	dirent;
	stringMap		map;
	std::string		path;
	stringPair		pair;

	while ((dirent = readdir(dir)))
	{
		path = relativePath;
		if (path[path.size() - 1] != '/')
			path += "/";
		path += dirent->d_name;
		pair = stringPair(path, dirent->d_name);
		map.insert(pair);
		path.clear();
	}
	closedir(dir);
	return map;
}

std::string	createListingDirectoryLink(stringMap & mapDir)
{
	std::string	list;

	for(stringMapIt it = mapDir.begin(); it != mapDir.end(); it++)
		list += "		<li><a href=\"" + (*it).first + "\">" + (*it).second + "</a></li>\n";
	return list;
}