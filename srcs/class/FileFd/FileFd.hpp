/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileFd.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/09/25 10:04:22 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP
# define FILE_HPP

#include "general.hpp"
#include "../Mime/Mime.hpp"

class FileFd
{
	private:
		int			_stockFd;
		int			_fileFd;
		int			_clientFd;
		size_t		_fileTotalSize;
		size_t		_fileSent;
		std::string	_buffer;
		std::string	_filePath;
		std::string	_fileType;
		std::string	_method;

		int			_status;
		bool		_EndFilepost;

		FileFd();

	public:
		static int			_sockFd[2];

		FileFd(int const & fileFd, int const & clientFd, std::string const & filePath);
		FileFd(FileFd const & other);
		FileFd & operator=(FileFd const & other);
		~FileFd();

		int			getStockFd() const;
		int			getFileFd() const;
		int			getClientFd() const;
		std::string	getBuffer() const;
		std::string	getFilePath() const;
		std::string	getFileType() const;
		std::string	getOriginMethod() const;
		size_t		getFileTotalSize() const;
		size_t		getFileSentSize() const;

		void		increaseFileSent(size_t	const & add);
		bool		get_endFilePost();
		void		set_endFilePost(bool stat);
		int			getStatus() const;
		void		setStatus(int const & status);

		void		setFileType();

		void		setBuffer(std::string const & content);
		void		clearBuffer();

		void		setupFd();
		void		manageFd();
		void		restoreFd();

		static void	initSockFd();
		static void	closeSockFd();
};

#endif