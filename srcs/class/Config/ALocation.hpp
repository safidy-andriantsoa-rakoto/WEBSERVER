/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ALocation.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:42:08 by barandri          #+#    #+#             */
/*   Updated: 2025/09/16 22:33:52 by trahanta         ###   ########.fr       */
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
		bool _checkStats;

	public:
		virtual	~ALocation(){};

		std::string	getAutoindex() const;
		std::string	getLocation() const;
		std::string	getRootDirectory() const;
		stringVect  getAvailableMethod() const;
		stringVect  getRedirect() const;
		stringVect  getIndex() const;

        virtual void	parseLocation(std::string const & locationString) = 0;
		  virtual void	parseLocation(const stringVect &file);

		void			displayLocation() const;
	
};

#endif