/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfig.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:26:52 by barandri          #+#    #+#             */
/*   Updated: 2025/08/11 15:01:51 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACONFIG_HPP
# define ACONFIG_HPP

#include "general.hpp"
#include "ALocation.hpp"

struct s_errPage
{
	stringVect	listError;
	std::string	content;
};

class AConfig
{
	protected:

		int			_port;
		std::string	_host;
		stringMap	_serverName;
		t_errPage	_errPage;
		ALocation	*_location;

	public:

		int			getServPort() const;
		std::string	getServHost() const;
		stringMap	getServName() const;
		t_errPage	getErrPage() const;
		ALocation	*getClassLocation() const;

		AConfig();
		virtual 	~AConfig(){};

		virtual void	parseConfig(std::string const & filePath) = 0;
};

#endif