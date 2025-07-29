/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 10:33:57 by barandri          #+#    #+#             */
/*   Updated: 2025/07/29 08:17:21 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// File error management + type of file
// Execute the file
// Send response to the server

void    execCgi(std::string const & path, char **envp, int & pid, int & status, int & fd)
{
	int				child_status;
	int				returnValue;
	char			*exec[3];
	std::ifstream	file;
	std::string		php;

	file.open(path.c_str());
	if (!file.is_open())
		throw std::runtime_error("File can't be opened");
	checkFileExtension(path, FILE_EXT);
	php = "/usr/bin/php-cgi";
	exec[0] = const_cast<char * >(php.c_str());
	exec[1] = const_cast<char * >(path.c_str());
	exec[2] = NULL;
	pid = fork();
	if (pid == -1)
		throw std::runtime_error("Fork error");
	if (pid == 0)
    {
		dup2(fd, STDOUT_FILENO);
        if (execve(php.c_str(), exec, envp) == -1)
		{
			file.close();
			close(fd);
			status = 1;
			std::cout << "Error on child exec" << std::endl;
		}
		return ;
    }
    returnValue = waitpid(pid, &child_status, 0);
	std::cout << "Exited status = " << WEXITSTATUS(child_status) << std::endl;
	if (returnValue == -1 || WIFEXITED(child_status) == false)
		throw std::runtime_error("File execution error");
	printFileContent(RESPONSE_FILE_NAME);
	close(fd);
	file.close();
}

int    callCgi(char const **argv, char **envp)
{
	int	pid;
	int status;
	int fd;

	status = 0;
	try
	{
		fd = open(RESPONSE_FILE_NAME, O_TRUNC| O_RDWR | O_CREAT, 0666);
		execCgi(argv[1], envp, pid, status, fd);
	}
	catch(const std::exception & e)
	{
		std::cerr << "	*	CGI ERROR !!!	*	" << std::endl <<  e.what() << std::endl;
		close(fd);
		return (status);
	}
	if (pid)
		std::cout << "	*	CGI SUCCESS	*	" << std::endl;
    return (status);
}