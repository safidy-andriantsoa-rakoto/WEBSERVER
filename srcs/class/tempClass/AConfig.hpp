/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfig.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:26:52 by barandri          #+#    #+#             */
/*   Updated: 2025/08/11 20:21:33 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACONFIG_HPP
# define ACONFIG_HPP

#include "general.hpp"
#include "ALocation.hpp"

//For test only
#include "MyLoc.hpp"

struct s_errPage
{
	stringVect	listError;
	std::string	fileContent;
	std::string	fileName;
};

typedef std::vector<MyLoc>  vectLocation;
typedef vectLocation::iterator  vectLocationIt;
typedef vectLocation::const_iterator  vectLocationcIt;

class AConfig
{
	protected:

		int				_port;
		std::string		_host;
		stringVect		_serverName;
		t_errPage		_errPage;
		vectLocation	_location;

	public:

		int				getServPort() const;
		std::string		getServHost() const;
		stringVect		getServName() const;
		t_errPage		getErrPage() const;
		vectLocation	getClassLocation() const;

		virtual 		~AConfig(){};

		virtual void	parseConfig(std::string const & filePath) = 0;
};

#endif