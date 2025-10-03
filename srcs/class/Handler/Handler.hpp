/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/10/03 08:13:10 by barandri         ###   ########.fr       */
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

	public:
		Handler();
		Handler(vectServer & vectServer);
		Handler(Handler const & other);
		Handler & operator=(Handler const & other);
		~Handler();

		int				identifyFd(int const & fd, int & index);

		vectServer & 	getVectServer();
		VClient &		getVectClient();
		vectFileFd &	getVectFileFd();
		vectCgiFd &		getVectCgiFd();

		vectServerIt	getServerIterator(int const & i);
		VClientIt		getClientIterator(int const & i);
		vectFileFdIt	getFileFdIterator(int const & i);
		vectCgiFdIt		getCgiFdIterator(int const & i);

		void			setVectServer(vectServer & vectServer);

		void			addToVectClient(Server const & server, int const & fd);
		void			addToVectFileFd(VClientIt & it, int const & fd);
		void			addToVectCgiFd(VClientIt & it, int const & fd);

		void			removeClient(int const & index, EpollCreation & epoll);
		void			removeClient(VClientIt & it, EpollCreation & epoll);
	
		void			removeFileFd(int const & index);
		void			removeFileFd(vectFileFdIt & it);
		vectFileFdIt	findFdInFileFd(vectFileFd & allFileFd, int const & fd);

		void			removeCgiFd(int const & index);
		void			removeCgiFd(vectCgiFdIt & it);
		vectCgiFdIt		findInCgiFd(vectCgiFd & allCgiFd, int const & fd);

		void			cleanFdAndAlloc();

		void			displayHandlerStat();
};

#endif