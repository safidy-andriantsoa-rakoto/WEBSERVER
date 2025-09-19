/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirExcept.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:48:39 by barandri          #+#    #+#             */
/*   Updated: 2025/09/19 08:29:23 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  REDIREXCEPT_HPP
# define REDIREXCEPT_HPP

#include "AExcept.hpp"

class RedirExcept : public AExcept
{
	public:
		std::string	_uri;
		std::string	_upperDir;

		RedirExcept(int const & status, std::string const & errorMsg, std::string const & uri);
		RedirExcept(int const & status, std::string const & errorMsg, std::string const & uri, std::string const & listDir);
		virtual ~RedirExcept() throw();
};

#endif