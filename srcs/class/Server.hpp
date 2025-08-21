/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:59 by barandri          #+#    #+#             */
/*   Updated: 2025/08/21 14:38:51 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "ErrorExcept.hpp"

#include "tempClass/MyConf.hpp"
#include "tempClass/MyLoc.hpp"

#include "general.hpp"
#include "typedef.hpp"

class Server
{
	private:
	// From Config file (need to review attributes)
		MyConf				_config;

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

		//Server config
		MyConf		getConfig() const;

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

		//Server utils
		vectLocationIt	findLongestMatch(std::string const & text, vectLocation & location);

		//For testing only !!!!
		void simpleInit(std::string const & servPath, std::string const & locationPath);

};

#endif