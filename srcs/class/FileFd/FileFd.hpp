/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileFd.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/10/15 10:20:46 by rsafidy-         ###   ########.fr       */
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
		std::string	_boundary;

			
		int			_status;
		bool		_EndFilepost;

		int			_fdMPFDT; // multipartdata
		
		FileFd();

	public:
		static int			_sockFd[2];

		FileFd(int const & fileFd, int const & clientFd, std::string const & filePath);
		FileFd(FileFd const & other);
		FileFd & operator=(FileFd const & other);
		~FileFd();

		int			getStockFd() const;
		int			getFd() const;
		int			getClientFd() const;
		std::string	getBuffer() const;
		std::string	getFilePath() const;
		std::string	getOriginMethod() const;

		void		setFileTotalSize(size_t size);
		void		setFileSentSize(size_t size);
		size_t		getFileTotalSize() const;
		size_t		getFileSentSize() const;
		

		void		increaseFileSent(size_t	const & add);
		bool		get_endFilePost();
		void		set_endFilePost(bool stat);

		int			getStatus() const;
		void		setStatus(int const & status);
		
		std::string	getFileType() const;
		void		setFileType();

		void		setBuffer(std::string const & content);
		void		clearBuffer();

		void		setupFd();
		void		manageFd();
		void		restoreFd();

		static void	initSockFd();
		static void	closeSockFd();

		void		set_post_body(std::string &body);
		void		set_boundary(std::string boundary);
		std::string &get_boundary();
	
		std::string get_range_body(int start, int end);
		void		delete_range_body(int start, int end);

};

#endif