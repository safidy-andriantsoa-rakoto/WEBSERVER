/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiFd.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/10/22 11:30:06 by barandri         ###   ########.fr       */
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

		bool		_isError;

		int			_readStat;
		std::string	_bufferRead;
		int			_statusCode;
		stringMap	_header;
		bool		_isHeader;
		std::string	_body;

		std::string	_bufferWrite;
		int			_writeStat;
		size_t		_pos;

		CgiFd();

	public:
		CgiFd(int const & cgiFd, int const & clientFd);
		CgiFd(int const &outputFd, int const & errorFd, int const & inputFd, int const &clientFd);
		CgiFd(CgiFd const & other);
		CgiFd & operator=(CgiFd const & other);
		~CgiFd();

		int			getOutputFd() const;
		void		setOutputFd(int const & fd);
	
		int			getErrFd() const;
		int			getInputFd() const;
		int			getClientFd() const;
		std::string	getBuffer() const;
		int			getReadStat() const;
		void		setReadStat(int const & status);

		bool		getIsError() const;
		void		setIsError(bool const & stat);

		void		setBufferRead(std::string const & content);
		void		bufferReadParser();
		void		clearBufferRead();

		std::string	getBody() const;

		std::string	getBufferWrite() const;
		void		setBufferWrite(std::string const & buff);

		int			getWriteStat() const;
		void		setWriteStat(int const & stat);

		size_t		getPos() const;
		void		setPos(size_t const & position);

		stringMap	getHeader() const;

		int			getStatusCode();
		void		setStatusCode(int const & statusCode);

		bool		getIsHeader();
		void		setIsHeader(bool const & stat);
	
		void		displayBufferReadParsed() const;

		void		checkError();

		std::string	getBufferToWrite();
};

#endif