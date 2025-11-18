/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyConf.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:58:12 by barandri          #+#    #+#             */
/*   Updated: 2025/11/17 14:33:53 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYCONF_HPP
# define MYCONF_HPP

#include "AConfig.hpp"

class MyConf : public AConfig
{
	private:
		bool	_checkStats;

		void	checkConfigElement(vectData & data);
		void	checkMandatoryElement(stringVect & vectElt);

		void	manageUploadLocation();

		void	parseHost(stringVect const & vect);
		void	parsePort(stringVect const & vect, intVect & allPort);
		void	parseErrorPage(stringVect const & vect);
		void	parseSize(stringVect const & vect);
		void	parseLocationBloc(vectDataIt & itUpper, vectData & data);

	public:
		MyConf();
		MyConf(MyConf const & other);
		MyConf & operator=(MyConf const & other);
		~MyConf();

		bool	getStatus();
		void	setHostName(std::string const & host);
		void	init_port_child(int const & port);
		void	delete_port_child_in_parent();

		void	parseConfig(vectData & data, intVect & allPort);

		void	displayConfig() const;
};

#endif