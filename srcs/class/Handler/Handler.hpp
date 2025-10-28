/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/10/28 09:50:49 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLER_HPP
# define HANDLER_HPP

#include "general.hpp"
#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../CgiFd/CgiFd.hpp"
#include "../Epoll/EpollCreation.hpp"

class Client;

typedef std::vector<Client>     VClient;
typedef VClient::iterator       VClientIt;

typedef std::vector<Server>		vectServer;
typedef vectServer::iterator	vectServerIt;

typedef std::vector<CgiFd *>	vectCgiFd;
typedef vectCgiFd::iterator		vectCgiFdIt;

class Handler
{
	private:
		vectServer	_vectServer;
		VClient		_vectClient;
		vectCgiFd	_vectCgiFd;

		bool		_programBreak;
		bool		_isChildProcess;

	public:
		Handler();
		Handler(vectServer & vectServer);
		Handler(Handler const & other);
		Handler & operator=(Handler const & other);
		~Handler();

		void			identifyFd(int const & fd, int & index, int & identifier);

		vectServer & 	getVectServer();
		VClient &		getVectClient();
		vectCgiFd &		getVectCgiFd();

		bool			getProgramBreak() const;
		void			setProgramBreak(bool const & stat);

		bool			getIsChildProcess() const;
		void			setIsChildProcess(bool const & stat);

		vectServerIt	getServerIterator(int const & i);
		VClientIt		getClientIterator(int const & i);
		vectCgiFdIt		getCgiFdIterator(int const & i);

		void			setVectServer(vectServer & vectServer);

		void			addToVectClient(Server const & server, int const & fd);
		void			addToVectCgiFd(VClientIt &it, int *fd, pid_t const & childPid);

		void			removeClient(int const & index, EpollCreation & epoll);
		void			removeClient(VClientIt & it, EpollCreation & epoll);
			
		void			removeCgiFd(int const & index);
		void			removeCgiFd(vectCgiFdIt & it);
		vectCgiFdIt		findFdInCgiFd(vectCgiFd & allCgiFd, int const & fd);
		vectCgiFdIt		findPtrInCgiFd(vectCgiFd & allCgiFd, CgiFd *ptr);
		
		void			cleanFdAndAlloc();
		
		void			displayHandlerStat();
		void			check_client_timeout_all(EpollCreation& epoll);
};

#endif