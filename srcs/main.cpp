/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:35:15 by barandri          #+#    #+#             */
/*   Updated: 2025/08/12 15:12:39 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main()
{
	// stringVect path;
    // DIR* dir = opendir("ext");
	// if (!dir)
	// {
	// 	std::cout << "Error\n dirent" << std::endl;
	// 	return (0);
	// }
    // struct dirent* entry;
    // while ((entry = readdir(dir)) != NULL)
	// {
	// 	std::string name(entry->d_name);
	// 	if (!name.compare(".") || !name.compare(".."))
	// 		continue;	
	// 	path.push_back("./ext/" + name);
    // }
    // closedir(dir);
	// for (size_t i = 0; i < path.size(); i++)
	// {
	// 	std::cout << path[i] << std::endl;
	// }
	// Mime test(path);
	try
	{
		Response::initAllStatusCode("ext/statcode/statusCode.txt");
		vectServer	allServer = parseConfigFile();
		launchServer(allServer);
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}