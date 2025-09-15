/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirExcept.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:48:39 by barandri          #+#    #+#             */
/*   Updated: 2025/09/15 14:54:11 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  DIREXCEPT_HPP
# define DIREXCEPT_HPP

#include "AExcept.hpp"
#include "../Config/AConfig.hpp"

class DirExcept : public AExcept
{
	public:
		stringVect	_dirTree;

		DirExcept(int const & status, DIR *dir, std::string const & path, std::string const & errMsg);
		virtual ~DirExcept() throw();
};

#endif