/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ALocation.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:42:08 by barandri          #+#    #+#             */
/*   Updated: 2025/11/06 15:09:12 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALOCATION_HPP
# define ALOCATION_HPP

#include "general.hpp"

struct s_redirection
{
	std::string filePath;
	int error;
};

class ALocation
{
	protected:
		stringVect		_autoindex;
		stringVect		_location;
		std::string 	_locations;
		stringVect		_root;
		stringVect		_method;
		stringVect		_redirect;
		stringVect		_index;
		stringVect		_cgiExtension;
		bool 			_checkStats;
		size_t 			_endConfig;
		t_redirection	_redirection;

	public:
		ALocation();
		ALocation(std::string const & locationName);
		ALocation(ALocation const & other);
		ALocation & operator=(ALocation const & other);
		virtual	~ALocation(){};

		std::string	getAutoindex() const;
		std::string	getLocation() const;
		std::string	getRootDirectory() const;
		std::string	getCgiActivation() const;
		std::string	getCgiExecPath() const;
		stringVect  getAvailableMethod() const;
		stringVect  getRedirect() const;
		stringVect  getIndex() const;

        virtual void	myParseLocation(std::string const & locationString) = 0;
		virtual void	parseLocation(const stringVect &file) = 0;

		void			displayLocation() const;
};

#endif