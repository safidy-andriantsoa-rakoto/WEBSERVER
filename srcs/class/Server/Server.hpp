/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:59 by barandri          #+#    #+#             */
/*   Updated: 2025/10/31 08:23:12 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "../Request/Request.hpp"
#include "../Response/Response.hpp"

#include "../Config/MyConf.hpp"
#include "../Config/MyLoc.hpp"

#include "general.hpp"
#include "typedef.hpp"
#include "../Mime/Mime.hpp"

class Server
{
	private:
        int     	_fdClient;
	    int     	_fdSocket;
        socklen_t  	_size_sockaddrin;
        SockAddrIn	_sockaddrIn;

	protected :
		MyConf		_config;

	public:
		Server();
		Server(Server const & other);
		Server & operator=(Server const & other);
		~Server();

		//Server config
		MyConf		getConfig() const;

		//Server connection
		int 		get_fdClient() const;
		int 		get_fdSocket() const;
		SockAddrIn	& get_confSockAddrIn();
        int 		create_socket();
        void 		configure_sockaddrin(int port);
        int 		link_socket_adress();
        int 		wait_connexion_from_client();
        int 		accept_client_connexion();
        std::string get_client_request();

		void		clearAllFd();

		static std::vector<int> stockPort(std::vector<Server> &allServer);
		static void getParsAllServer(std::vector<Server> &allServer, stringVect file);
		static bool checkDuplicateValue(std::vector<Server> allserver);

		/* [Test only] */
		void simpleInit(std::string const & servPath, std::string const & locationPath);
};

#endif