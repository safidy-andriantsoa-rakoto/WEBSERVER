/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ALocation.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:42:08 by barandri          #+#    #+#             */
/*   Updated: 2025/10/20 14:09:47 by trahanta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALOCATION_HPP
# define ALOCATION_HPP

#include "general.hpp"

class ALocation
{
	protected:

		stringVect	_autoindex;
		stringVect	_location;
		std::string _locations;
		stringVect	_root;
		stringVect	_method;
		stringVect	_redirect;
		stringVect	_index;
		bool 		_checkStats;
		size_t 		_endConfig;


	public:
		ALocation();
		ALocation(ALocation const & other);
		ALocation & operator=(ALocation const & other);
		virtual	~ALocation(){};

		void 		initializeValue(size_t value);
		std::string	getAutoindex() const;
		std::string	getLocation() const;
		std::string	getRootDirectory() const;
		stringVect  getAvailableMethod() const;
		stringVect  getRedirect() const;
		stringVect  getIndex() const;

        virtual void	myParseLocation(std::string const & locationString) = 0;
		virtual void	parseLocation(const stringVect &file) = 0;

		void			displayLocation() const;
};

#endif
