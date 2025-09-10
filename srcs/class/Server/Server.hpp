/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:34:59 by barandri          #+#    #+#             */
/*   Updated: 2025/09/10 14:07:36 by barandri         ###   ########.fr       */
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
	// From Config file (need to review attributes)
		MyConf				_config;

	// For socket
	    int     			_fdSocket;
        int     			_fdClient;
        socklen_t  			_size_sockaddrin;
        SockAddrIn			_sockaddrIn;

		//HTTP Method
		int		get_request(Request & request);
		int		post_request(Request & request);
		void	delete_request(Request & request);

		//Server utils functionnality
		void		processRequestWithConfig(Request & request);
		void		checkContentLength(Request & request);

		void		buildRequestTarget(Request & request);
	
		//Location requirements
		void		processLocationOption(MyLoc const & location, Request & request);
		void		checkMethodInLocation(stringVect & locationMethod, std::string const & method);
		void		processRedirection(MyLoc const & location, Request & request);
	

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
		int processRequest(std::string const & clientRequest, int const & fd_cli, int & opt, std::string & filePath);

		//Server utils
		vectLocationIt	findLongestMatch(std::string const & text, vectLocation & location);

		//For testing only !!!!
		void simpleInit(std::string const & servPath, std::string const & locationPath);

};

#endif