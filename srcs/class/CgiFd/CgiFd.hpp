/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiFd.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/10/06 09:14:10 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIFD_HPP
# define CGIFD_HPP

#include "general.hpp"

class CgiFd
{
	private:
		int			_cgiFd;
		int			_cgiErrFd;
		int			_clientFd;
		int			_status;
		std::string	_buffer;

		bool		_isError;

		CgiFd();

	public:
		CgiFd(int const & cgiFd, int const & clientFd);
		CgiFd(int const & cgiFd, int const & cgiErrFd, int const & clientFd);
		CgiFd(CgiFd const & other);
		CgiFd & operator=(CgiFd const & other);
		~CgiFd();

		int			getFd() const;
		int			getErrFd() const;
		int			getClientFd() const;
		std::string	getBuffer() const;
		int			getStatus() const;
		void		setStatus(int const & status);

		bool		getIsError() const;
		void		setIsError(bool const & stat);

		void		setBuffer(std::string const & content);
		void		clearBuffer();
};

#endif