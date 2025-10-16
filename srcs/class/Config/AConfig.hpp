/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfig.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:26:52 by barandri          #+#    #+#             */
/*   Updated: 2025/10/15 10:32:56 by barandri         ###   ########.fr       */
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

//For test only
#include "MyLoc.hpp"

typedef std::vector<MyLoc>				vectLocation;
typedef vectLocation::iterator			vectLocationIt;
typedef vectLocation::const_iterator	vectLocationcIt;

class AConfig
{
	protected:

		int				_port;
		float			_maxSize;
		std::string		_host;
		stringVect		_serverName;
		t_errPage		_errPage;
		bool			_hasErrPage;
		vectLocation	_location;
		stringVect		_size;

	public:
		AConfig();
		AConfig(AConfig const & other);
		AConfig &		operator=(AConfig const &other);

		int				getServPort() const;
		float			getMaxSize() const;
		std::string		getServHost() const;
		stringVect		getServName() const;
		t_errPage		getErrPage() const;
		vectLocation	getClassLocation() const;

		vectLocationIt	findClassLocation(std::string const & toFind);

		virtual 		~AConfig(){};

		virtual void	locationParsing(std::string const & filePath) = 0;
};

#endif