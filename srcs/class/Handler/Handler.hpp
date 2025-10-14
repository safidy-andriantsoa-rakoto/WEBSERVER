/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/10/14 14:39:59 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLER_HPP
# define HANDLER_HPP

#include "general.hpp"
#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../FileFd/FileFd.hpp"
#include "../CgiFd/CgiFd.hpp"
#include "../Epoll/EpollCreation.hpp"

class Client;

typedef std::vector<Client>     VClient;
typedef VClient::iterator       VClientIt;

typedef std::vector<Server>		vectServer;
typedef vectServer::iterator	vectServerIt;

typedef std::vector<FileFd *>	vectFileFd;
typedef vectFileFd::iterator	vectFileFdIt;

typedef std::vector<CgiFd *>	vectCgiFd;
typedef vectCgiFd::iterator		vectCgiFdIt;

class Handler
{
	private:
		vectServer	_vectServer;
		VClient		_vectClient;
		vectFileFd	_vectFileFd;
		vectCgiFd	_vectCgiFd;

		bool		_programBreak;

	public:
		Handler();
		Handler(vectServer & vectServer);
		Handler(Handler const & other);
		Handler & operator=(Handler const & other);
		~Handler();

		void			identifyFd(int const & fd, int & index, int & identifier);

		vectServer & 	getVectServer();
		VClient &		getVectClient();
		vectFileFd &	getVectFileFd();
		vectCgiFd &		getVectCgiFd();

		bool			getProgramBreak();
		void			setProgramBreak(bool const & stat);

		vectServerIt	getServerIterator(int const & i);
		VClientIt		getClientIterator(int const & i);
		vectFileFdIt	getFileFdIterator(int const & i);
		vectCgiFdIt		getCgiFdIterator(int const & i);

		void			setVectServer(vectServer & vectServer);

		void			addToVectClient(Server const & server, int const & fd);
		void			addToVectFileFd(VClientIt & it, int const & fd);
		void			addToVectCgiFd(VClientIt &it, int const &fdOut, int const & fdErr, int const & fdIn);

		void			removeClient(int const & index, EpollCreation & epoll);
		void			removeClient(VClientIt & it, EpollCreation & epoll);
	
		void			removeFileFd(int const & index);
		void			removeFileFd(vectFileFdIt & it);
		void			checkAndRemoveFileFd(FileFd *ptr);
		vectFileFdIt	findFdInFileFd(vectFileFd & allFileFd, int const & fd);
		vectFileFdIt	findPtrInFileFd(vectFileFd & allFileFd, FileFd *ptr);
		
		void			removeCgiFd(int const & index);
		void			removeCgiFd(vectCgiFdIt & it);
		void			checkAndRemoveCgiFd(CgiFd *ptr);
		vectCgiFdIt		findFdInCgiFd(vectCgiFd & allCgiFd, int const & fd);
		vectCgiFdIt		findPtrInCgiFd(vectCgiFd & allCgiFd, CgiFd *ptr);
		
		void			cleanFdAndAlloc();
		
		void			displayHandlerStat();
};

#endif