/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorExcept.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:48:39 by barandri          #+#    #+#             */
/*   Updated: 2025/08/25 15:18:48 by barandri         ###   ########.fr       */
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
		std::string	_redir;

		ErrorExcept(int const & status, std::string const & error);
		ErrorExcept(int const & status, std::string const & error, std::string const & redir);
		~ErrorExcept() throw();
};

#endif