/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileFd.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/09/15 15:12:47 by barandri         ###   ########.fr       */
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
		std::string	_buffer;
		std::string	_filePath;
		std::string	_fileType;
		std::string	_method;

		int			_status;

	public:
		static int			_sockFd[2];

		FileFd();
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

		int			getStatus() const;
		void		setStatus(int const & status);

		void		setFileType();

		void		setBuffer(std::string const & content);

		void		setupFd();
		void		manageFd();
		void		restoreFd();

		static void	initSockFd();
		static void	closeSockFd();
};

#endif