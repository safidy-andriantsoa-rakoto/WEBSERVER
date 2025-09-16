/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyLoc.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:58:31 by barandri          #+#    #+#             */
/*   Updated: 2025/09/16 22:33:14 by trahanta         ###   ########.fr       */
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

		virtual void	parseLocation(std::string const & filePath);
		virtual void	parseLocation(const stringVect &file);
		bool getStatus();
};

#endif