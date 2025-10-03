/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiFd.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/10/03 07:53:41 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIFD_HPP
# define CGIFD_HPP

#include "general.hpp"

class CgiFd
{
	private:
		int			_CgiFd;
		int			_clientFd;
		int			_status;
		std::string	_buffer;

		CgiFd();

	public:
		CgiFd(int const & CgiFd, int const & clientFd);
		CgiFd(CgiFd const & other);
		CgiFd & operator=(CgiFd const & other);
		~CgiFd();

		int			getFd() const;
		int			getClientFd() const;
		std::string	getBuffer() const;
		int			getStatus() const;
		void		setStatus(int const & status);

		void		setBuffer(std::string const & content);
		void		clearBuffer();
};

#endif