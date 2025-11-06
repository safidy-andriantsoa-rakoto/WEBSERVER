/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyLoc.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:58:31 by barandri          #+#    #+#             */
/*   Updated: 2025/11/06 15:01:52 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYLOC_HPP
# define MYLOC_HPP

#include "ALocation.hpp"

class MyLoc : public ALocation
{
	private:
		void	checkLocElement(stringVect const &vect);

		void	parseRoot(stringVect & vect);
		void	parseMethod(stringVect & vect);
		void	parseAutoIndex(stringVect & vect);
		void	parseCgiExt(stringVect & vect);
		void	parseRedirect(stringVect & vect);

	public:
		MyLoc();
		MyLoc(std::string const & locName);
		MyLoc(MyLoc const & other);
		MyLoc & operator=(MyLoc const & other);
		~MyLoc();

		bool			getStatus();
		void 			getElementLocation(const size_t &start, const size_t &end, const stringVect &file);

		virtual void	parseLocation(const stringVect &file);

		void			oldParseLocation(stringVect const & file);

		//barandri_function
		virtual void	myParseLocation(std::string const & filePath);
};

#endif