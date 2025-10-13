/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ALocation.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:42:08 by barandri          #+#    #+#             */
/*   Updated: 2025/10/08 20:40:31 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALOCATION_HPP
# define ALOCATION_HPP

#include "general.hpp"

class ALocation
{
	protected:

		stringVect _autoindex;
		stringVect _location;
		stringVect _root;
		stringVect _method;
		stringVect _redirect;
		stringVect _index;

	public:
		ALocation();
		ALocation(ALocation const & other);
		ALocation & operator=(ALocation const & other);
		virtual	~ALocation(){};

		std::string	getAutoindex() const;
		std::string	getLocation() const;
		std::string	getRootDirectory() const;
		stringVect  getAvailableMethod() const;
		stringVect  getRedirect() const;
		stringVect  getIndex() const;

        virtual void	parseLocation(std::string const & locationString) = 0;

		void			displayLocation() const;
};

#endif