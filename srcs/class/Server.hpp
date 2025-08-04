/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:59 by barandri          #+#    #+#             */
/*   Updated: 2025/08/04 15:02:14 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "general.hpp"

class Server
{
	private:
	// From Config file (need to review attributes)
		std::string			_port;
		std::string			_host;
		std::string			_serverName;
		std::string			_generalRoot;
		stringMap			_location;

	// For socket
	    int     			_fdSocket;
        int     			_fdClient;
        size_t  			_size_sockaddrin;
        struct  sockaddr_in	_sockaddrIn;

		//HTTP Method
		Response	get_request(Request & request);
		Response	post_request(Request & request);
		Response	delete_request(Request & request);

		//Server utils functionnality
		void		processRequestWithConfig(Request & request);
		void		buildRequestTarget(Request & request);
		void		sendResponse(std::string const & requestTarget, Response & response);
	
	public:
		Server();
		Server(Server & other);
		Server & operator=(Server & other);
		~Server();

		std::string getPort() const;
		std::string getHost() const;
		std::string getServerName() const;
		std::string	getGeneralRoot() const;
		stringMap	getLocation() const;

		//Server connection
		int 		get_fdClient();
        int 		create_socket();
        void 		configure_sockaddrin(int port);
        int 		link_socket_adress();
        int 		wait_connexion_from_client();
        int 		accept_client_connexion();
        std::string get_client_request();

		//Server main functionnality
		void    processRequest(std::string const & clientRequest);

		//For testing only !!!!
		void		simpleInit();
};

#endif