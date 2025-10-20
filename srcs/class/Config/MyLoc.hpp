/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyLoc.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:58:31 by barandri          #+#    #+#             */
/*   Updated: 2025/10/20 14:12:09 by trahanta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYLOC_HPP
# define MYLOC_HPP

#include "ALocation.hpp"

class MyLoc : public ALocation
{
	public:
		MyLoc();
		MyLoc(MyLoc const & other);
		MyLoc & operator=(MyLoc const & other);
		~MyLoc();

		bool	getStatus();
		void 	getElementLocation(const size_t &start, const size_t &end, const stringVect &file);

		virtual void	myParseLocation(std::string const & locationString);
		virtual void	parseLocation(const stringVect &file);

	
};

#endif
