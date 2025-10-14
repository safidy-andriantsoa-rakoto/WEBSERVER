/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiFd.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/10/14 14:38:56 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIFD_HPP
# define CGIFD_HPP

#include "general.hpp"
#include "../Response/ErrorExcept.hpp"

class CgiFd
{
	private:
		int			_outputFd;
		int			_errorFd;
		int			_inputFd;
		int			_clientFd;
		int			_status;

		bool		_isError;

		std::string	_buffer;
		int			_statusCode;
		stringMap	_header;
		bool		_isHeader;
		std::string	_body;

		CgiFd();

	public:
		CgiFd(int const & cgiFd, int const & clientFd);
		CgiFd(int const &outputFd, int const & errorFd, int const & inputFd, int const &clientFd);
		CgiFd(CgiFd const & other);
		CgiFd & operator=(CgiFd const & other);
		~CgiFd();

		int			getOutputFd() const;
		int			getErrFd() const;
		int			getInputFd() const;
		int			getClientFd() const;
		std::string	getBuffer() const;
		int			getStatus() const;
		void		setStatus(int const & status);

		bool		getIsError() const;
		void		setIsError(bool const & stat);

		void		setBuffer(std::string const & content);
		void		bufferParser();
		void		clearBuffer();

		std::string	getBody() const;

		stringMap	getHeader() const;

		int			getStatusCode();
		void		setStatusCode(int const & statusCode);

		bool		getIsHeader();
		void		setIsHeader(bool const & stat);
	
		void		displayBufferParsed() const;

		void		checkError();
};

#endif