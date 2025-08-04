/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorExcept.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:50:48 by barandri          #+#    #+#             */
/*   Updated: 2025/08/04 22:19:43 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorExcept.hpp"

ErrorExcept::ErrorExcept(int const & status, std::string const & error) : _status(status), _error(error){}

ErrorExcept::~ErrorExcept() throw(){}
