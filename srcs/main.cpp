/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/10/16 14:49:38 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


// int main()
// {
// 	try
// 	{
// 		std::string text = readFileToString("test.txt");
// 		std::stringstream	ss;
// 		ss << text;
// 		std::string path = "test.txt";
// 		char buff[50];
// 		std::string bufferStr;
// 		while(1)
// 		{
// 			ss >> buff;
// 			bufferStr.append(buff);
// 			if (bufferStr.size() == 0)
// 				break;
// 			std::cout << std::endl << "*********************" << bufferStr.size() << "*********************\n" << bufferStr;
// 			bufferStr.clear();
// 		}
// 		// size_t		pos = 0;
// 		// size_t		size = 148;
// 		// while(1)
// 		// {
			
// 		// 	div = divideStringBySize(text, pos, size);
// 		// 	std::cout << std::endl << "-----------------Div size = " << div.size() << std::endl;
// 		// 	std::cout << div;
// 		// 	pos += size;
// 		// 	if (pos > text.size())
// 		// 	{
// 		// 		std::cout << std::endl << "[EOF]" << std::endl;
// 		// 		break;
// 		// 	}
// 		// 	div.clear();
// 		// }
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << e.what() << '\n';
// 	}
	
// }

/*
	Check every configFile needed by the program !!!
*/
static void	initProgram()
{
	Response::initAllStatusCode(STAT_PATH);
	Mime::initMapMime(MIME_PATH);
	LDirExcept::initPart(LDIR_PATH);
	CgiParser::initCgiConfig(CGI_GET_PATH, CGI_POST_PATH, CGI_ERR_PATH);
	int fd = openFile(ERR_PATH);
	close(fd);
	DIR *dir = openDir(UPLOAD_PATH);
	closedir(dir);
}

int main()
{
	Handler handler;

	try
	{
		initProgram();
		vectServer	allServer = myParseConfigFile();
		handler.setVectServer(allServer);
		launchServer(handler);
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		handler.cleanFdAndAlloc();
		return 1;
	}
	return 0;
}

// int main(int argc, char **argv)
// {
// 	Handler handler;

// 	(void ) argc;
// 	(void) argv;
// 	try
// 	{
// 		initProgram();
// 		stringVect		file;
// 		vectLocation	Location;
// 		vectServer 		server;
		
// 		file = getContentFile("config/serverConf/test.conf");
// 		if (!checkBraceClose(file))
// 			return (1);
// 		if(!checkBracesAfterNewline(file))
// 			return (1);
// 		if (!checkWriteConfig(file, "server", "server {"))
// 			return (1);
// 		if(!checkNumberAllContent(file))
// 			return (1);
// 		vectServer	allServer = getServer(file);
// 		Server::getParsAllServer(allServer, file);
// 		// handler.setVectServer(allServer);
// 		// launchServer(handler);
// 	}
// 	catch(const std::exception & e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 		handler.cleanFdAndAlloc();
// 		return -1;
// 	}
// 	return 0;
// }
