/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:59 by barandri          #+#    #+#             */
/*   Updated: 2025/07/28 20:41:48 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "general.hpp"
#include "Request.hpp"

class Server
{
	private:
	// From Config file (need to review attributes)
		std::string			_port;
		std::string			_host;
		std::string			_serverName;
		std::string			_root;
		std::string			_location;

	// For socket
	    int     			_fdSocket;
        int     			_fdClient;
        size_t  			_size_sockaddrin;
        struct  sockaddr_in	_sockaddrIn;

	public:
		Server();
		Server(Server & other);
		Server & operator=(Server & other);
		~Server();

		std::string getPort() const;
		std::string getHost() const;
		std::string getServerName() const;
		std::string getRoot() const;
		std::string getLocation() const;

		int 		get_fdClient();
        int 		create_socket();
        void 		configure_sockaddrin(int port);
        int 		link_socket_adress();
        int 		wait_connexion_from_client();
        int 		accept_client_connexion();
        std::string get_client_request();

		//HTTP Method
		void 		get_request(Request & request, int &_acceptStatus);
};

#endif