/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorExcept.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:48:39 by barandri          #+#    #+#             */
/*   Updated: 2025/08/27 09:06:28 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  ERROREXCEPT_HPP
# define ERROREXCEPT_HPP

#include "AExcept.hpp"
#include "../Config/AConfig.hpp"

class ErrorExcept : public AExcept
{
	public:
		ErrorExcept(int const & status, std::string const & errorMsg);
		virtual ~ErrorExcept() throw();
};

#endif