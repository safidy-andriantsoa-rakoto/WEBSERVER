/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyLoc.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:58:31 by barandri          #+#    #+#             */
/*   Updated: 2025/10/15 10:35:42 by barandri         ###   ########.fr       */
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

		virtual void	parseLocation(const stringVect &file);

		//barandri_function
		virtual void	myParseLocation(std::string const & filePath);
};

#endif