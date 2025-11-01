/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiFd.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/10/31 09:56:10 by barandri         ###   ########.fr       */
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

		time_t		_startTime;
		pid_t		_childPid;
		bool		_isChildAlive;

		bool		_firstCheckError;

		CgiFd();

	public:
		CgiFd(int *fd, pid_t const & childPid, int const & clientFd);
		CgiFd(CgiFd const & other);
		CgiFd & operator=(CgiFd const & other);
		~CgiFd();

		int			getOutputFd() const;
		void		setOutputFd(int const & fd);
	
		int			getInputFd() const;
		void		setInputFd(int const & fd);

		int			getClientFd() const;
		std::string	getBufferRead() const;
		int			getReadStat() const;
		void		setReadStat(int const & status);

		bool		getIsError() const;
		void		setIsError(bool const & stat);

		void		processBufferRead(std::string const & content);
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

		void		checkCgiOutput();

		std::string	operateOnBufferToWrite();

		void		checkTimeOut();

		pid_t		getChildPid() const;
		bool		getIsChildAlive() const;
		void		setIsChildAlive(bool const & stat);
		void		checkChildProcess();

};

#endif