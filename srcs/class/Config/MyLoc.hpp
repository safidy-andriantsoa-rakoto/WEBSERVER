/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyLoc.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:58:31 by barandri          #+#    #+#             */
/*   Updated: 2025/11/07 18:02:55 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYLOC_HPP
# define MYLOC_HPP

#include "ALocation.hpp"

class MyLoc : public ALocation
{
	private:
		void	checkLocElement(vectData & data);
		void	checkMandatoryElement(stringVect & vect);

		void	parseRoot(stringVect & vect);
		void	parseMethod(stringVect & vect);
		void	parseAutoIndex(stringVect & vect);
		void	parseCgiExt(stringVect & vect);
		void	parseRedirect(stringVect & vect);

		void 	getElementLocation(const size_t &start, const size_t &end, const stringVect &file);

	public:
		MyLoc();
		MyLoc(std::string const & locName);
		MyLoc(MyLoc const & other);
		MyLoc & operator=(MyLoc const & other);
		~MyLoc();

		void			specialParseLocation(stringVect const & file);
		virtual void	parseLocation(vectData & data);

};

#endif