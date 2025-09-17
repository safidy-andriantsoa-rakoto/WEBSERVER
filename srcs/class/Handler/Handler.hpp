/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/09/17 10:27:33 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLER_HPP
# define HANDLER_HPP

#include "general.hpp"
#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../FileFd/FileFd.hpp"

class Client;

typedef std::vector<Client>     VClient;
typedef VClient::iterator       VClientIt;

typedef std::vector<Server>		vectServer;
typedef vectServer::iterator	vectServerIt;

typedef std::vector<FileFd>		vectFileFd;
typedef vectFileFd::iterator	vectFileFdIt;

class Handler
{
	private:
		vectServer	_vectServer;
		VClient		_vectClient;
		vectFileFd	_vectFileFd;

	public:
		Handler();
		Handler(vectServer & vectServer);
		Handler(Handler const & other);
		Handler & operator=(Handler const & other);
		~Handler();

		std::string	identifyFd(int const & fd, int & index);

		vectServer & 	getVectServer();
		VClient &		getVectClient();
		vectFileFd &	getVectFileFd();

		vectServerIt	getServer(int const & i);
		VClientIt		getClient(int const & i);
		vectFileFdIt	getFileFd(int const & i);

		void			setVectServer(vectServer & vectServer);

		void	addToVectClient(Server const & server, int const & fd);
		void	addToVectFileFd(int const & fd, int const & fdCli, std::string const & filePath);

		void	removeClient(int const & index);
		void	removeFileFd(int const & index);

		void	closeAllFd();

		void	displayHandlerStat();
};

#endif