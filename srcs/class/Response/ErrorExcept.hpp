/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorExcept.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:48:39 by barandri          #+#    #+#             */
/*   Updated: 2025/10/24 13:21:32 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  ERROREXCEPT_HPP
# define ERROREXCEPT_HPP

#include "AExcept.hpp"
#include "../Config/AConfig.hpp"

class ErrorExcept : public AExcept
{
	public:
		std::string	_errPagePath;
		std::string	_body;

		ErrorExcept(int const & status, std::string const & errorMsg);
		ErrorExcept(int const & status, std::string const & body, std::string const & errorMsg);
		virtual ~ErrorExcept() throw();
};

#endif