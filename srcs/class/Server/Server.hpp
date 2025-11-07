/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:59 by barandri          #+#    #+#             */
/*   Updated: 2025/11/07 20:57:52 by barandri         ###   ########.fr       */
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
        int     		_fdClient;
	    int     		_fdSocket;
        socklen_t  		_size_sockaddrin;
        SockAddrIn		_sockaddrIn;
		struct addrinfo	_addrinfo;
		struct addrinfo	*_resp;

	protected :
		MyConf			_config;

	public:
		Server();
		Server(MyConf conf);
		Server(Server const & other);
		Server & operator=(Server const & other);
		~Server();

		MyConf			getConfig() const;
	
		int 			get_fdClient() const;
		int 			get_fdSocket() const;
		SockAddrIn		& get_confSockAddrIn();
		MyConf& 		config_ref();
	
        int 			create_socket();
        void 			configure_sockaddrin(int port);
        int 			link_socket_adress();
		int				resolve_host(std::string const & host, int const & port);
        int 			wait_connexion_from_client();
        int 			accept_client_connexion();
	
        std::string 	get_client_request();
	
		void			clearAllFd();
};

#endif