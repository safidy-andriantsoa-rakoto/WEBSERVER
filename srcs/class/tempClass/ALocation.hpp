/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ALocation.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:42:08 by barandri          #+#    #+#             */
/*   Updated: 2025/08/11 20:13:41 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALOCATION_HPP
# define ALOCATION_HPP

#include "general.hpp"

class ALocation
{
	protected:

		bool        _autoindex;
		std::string _location;
		std::string _root;
		stringVect  _method;
		stringVect  _redirect;
		stringVect  _index;

	public:
		virtual	~ALocation(){};

		bool        getAutoindex() const;
		std::string getLocation() const;
		std::string getRootDirectory() const;
		stringVect  getAvailableMethod() const;
		stringVect  getRedirect() const;
		stringVect  getIndex() const;

        virtual void	parseLocation(std::string const & filePath) = 0;

		void	displayLocation() const;
};

#endif