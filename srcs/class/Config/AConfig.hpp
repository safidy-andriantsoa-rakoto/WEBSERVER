/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfig.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:26:52 by barandri          #+#    #+#             */
/*   Updated: 2025/11/07 20:32:12 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACONFIG_HPP
# define ACONFIG_HPP

#include "general.hpp"
#include "ALocation.hpp"


struct s_errPage
{
	stringVect	strError;
	stringVect	listError;
	std::string	fileContent;
	std::string	filePath;
};

#include "MyLoc.hpp"

typedef std::vector<MyLoc>				vectLocation;
typedef vectLocation::iterator			vectLocationIt;
typedef vectLocation::const_iterator	vectLocationcIt;

class AConfig
{
	protected:

		intVect			_port;
		float			_maxSize;
		std::string		_host;
		t_errPage		_errPage;
		bool			_hasErrPage;
		vectLocation	_location;
		stringVect		_size;

	public:
		AConfig();
		AConfig(AConfig const & other);
		AConfig &		operator=(AConfig const &other);
		virtual 		~AConfig(){};

		intVect			getServPort() const;
		float			getMaxSize() const;
		std::string		getServHost() const;
		t_errPage		getErrPage() const;
		vectLocation	getClassLocation() const;

		vectLocationIt	findClassLocation(std::string const & toFind);

		virtual void	parseConfig(vectData & data, intVect & allPort) = 0;
};

#endif