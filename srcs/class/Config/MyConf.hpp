/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyConf.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:58:12 by barandri          #+#    #+#             */
/*   Updated: 2025/11/06 14:28:52 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYCONF_HPP
# define MYCONF_HPP

#include "AConfig.hpp"

class MyConf : public AConfig
{
	private:
		bool	_checkStats;
		int		_countLocation;
		int		_endLocation;

		void	checkConfigElement(parseMap & map);

		void	parseHost(stringVect const & vect);
		void	parsePort(stringVect const & vect, intVect & allPort);
		void	parseErrorPage(stringVect const & vect);
		void	parseSize(stringVect const & vect);
		void	parseLocationBloc(parseMapIt & it);

	public:
		MyConf();
		MyConf(MyConf const & other);
		MyConf & operator=(MyConf const & other);
		~MyConf();

		bool			getStatus();
		void			getElementConfig(const size_t &start, const size_t &end, const stringVect &file);
		void 			parseConfig(stringVect file);
		size_t			findNotDigitError();
		vectLocation	createVectLocation(size_t countLocation);
		void			setHostName(std::string host);
		//For testing
		virtual void	displayConfig() const;

		void			init_port_child(int port);
		void			delete_port_child_in_parent();

		/* [UPDATE] */
		void			parseNewConfig(parseMap & map, intVect & allPort);

		std::string		getElt(parseMapIt & it);
		std::string		getLocValue(parseMapIt & it);
		stringVect&		getLocVect(parseMapIt & it);
};

#endif