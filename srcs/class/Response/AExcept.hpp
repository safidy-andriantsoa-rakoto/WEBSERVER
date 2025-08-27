/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AExcept.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:48:39 by barandri          #+#    #+#             */
/*   Updated: 2025/08/27 09:08:31 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  AEXCEPT_HPP
# define AEXCEPT_HPP

#include "general.hpp"

class AExcept : public std::exception
{
	public:
		int			_statusCode;
		std::string	_errMsg;

		virtual ~AExcept() throw();
};

#endif