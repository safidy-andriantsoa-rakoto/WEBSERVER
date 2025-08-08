/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:59 by barandri          #+#    #+#             */
/*   Updated: 2025/08/08 08:48:47 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "ErrorExcept.hpp"
#include "general.hpp"
#include "typedef.hpp"

class Server
{
	private:
	// From Config file (need to review attributes)
		int					_port;
		std::string			_host;
		std::string			_serverName;
		std::string			_generalRoot;
		stringMap			_location;

	// For socket
	    int     			_fdSocket;
        int     			_fdClient;
        size_t  			_size_sockaddrin;
        SockAddrIn			_sockaddrIn;

		//HTTP Method
		Response	get_request(Request & request);
		Response	post_request(Request & request);
		Response	delete_request(Request & request);

		//Server utils functionnality
		void		processRequestWithConfig(Request & request);
		void		buildRequestTarget(Request & request);
	
	public:
		Server();
		Server(Server const & other);
		Server & operator=(Server const & other);
		~Server();

		int 		getPort() const;
		std::string getHost() const;
		std::string getServerName() const;
		stringMap	getLocation() const;

		//Server connection
		int 		get_fdClient() const;
		int 		get_fdSocket() const;
		SockAddrIn	get_confSockAddrIn() const;
        int 		create_socket();
        void 		configure_sockaddrin(int port);
        int 		link_socket_adress();
        int 		wait_connexion_from_client();
        int 		accept_client_connexion();
        std::string get_client_request();

		//Server main functionnality
		void    processRequest(std::string const & clientRequest, int const & fd_cli);

		//For testing only !!!!
		void simpleInit(int const & port, std::string const & locationPath);

};

#endif