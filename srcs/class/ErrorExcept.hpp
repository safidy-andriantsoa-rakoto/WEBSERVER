/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorExcept.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:48:39 by barandri          #+#    #+#             */
/*   Updated: 2025/08/04 22:19:08 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  ERROREXCEPT_HPP
# define ERROREXCEPT_HPP

#include "general.hpp"

class ErrorExcept : public std::exception
{
	public:
		int			_status;
		std::string	_error;

		ErrorExcept(int const & status, std::string const & error);
		~ErrorExcept() throw();
};

#endif