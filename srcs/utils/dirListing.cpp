/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dirListing.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:25:59 by barandri          #+#    #+#             */
/*   Updated: 2025/09/15 14:57:07 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "general.hpp"

stringVect	listingDirectory(DIR* dir, std::string const & path)
{
	struct dirent*	tmp;
	stringVect		vect;

	while ((tmp = readdir(dir)))
	{
		vect.push_back(path + tmp->d_name);
	}
	closedir(dir);
	return vect;
}