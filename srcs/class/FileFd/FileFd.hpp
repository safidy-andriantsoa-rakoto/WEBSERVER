/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileFd.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/09/09 10:26:26 by rsafidy-         ###   ########.fr       */
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
		int			_sockFd[2];
		int			_clientFd;
		std::string	_buffer;
		std::string	_filePath;
		std::string	_fileType;
		std::string	_method;

	public:
		FileFd();
		FileFd(int const & fileFd, int const & clientFd, std::string const & filePath);
		FileFd(FileFd const & other);
		FileFd & operator=(FileFd const & other);
		~FileFd();

		int			getStockFd() const;
		int			getFileFd() const;
		int*		getSockFd();
		int			getClientFd() const;
		std::string	getBuffer() const;
		std::string	getFilePath() const;
		std::string	getFileType() const;
		std::string	getOriginMethod() const;

		void		setFileType();

		void		setBuffer(std::string const & content);

		void		setupFd();
		void		manageFd();
		void		closeAllOwnedFileFd();
};

#endif