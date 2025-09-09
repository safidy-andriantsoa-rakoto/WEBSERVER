/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:46 by barandri          #+#    #+#             */
/*   Updated: 2025/09/08 15:10:47 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLER_HPP
# define HANDLER_HPP

#include "general.hpp"
#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../FileFd/FileFd.hpp"

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
		Handler(vectServer & vectServer);
		Handler(Handler const & other);
		Handler & operator=(Handler const & other);
		~Handler();

		std::string	identifyFd(int const & fd, int & index);

		vectServer & 	getVectServer();
		VClient &		getVectClient();
		vectFileFd &	getVectFileFd();

		void	addToVectClient(Server const & server, int const & fd);
		void	addToVectFileFd(int const & fd, int const & fdCli, std::string const & filePath);

		void	removeClient(int const & index);
		void	removeFileFd(int const & index);

		void	displayHandlerStat();
};

#endif