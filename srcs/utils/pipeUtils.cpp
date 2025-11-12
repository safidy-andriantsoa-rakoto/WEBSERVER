/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 14:23:44 by barandri          #+#    #+#             */
/*   Updated: 2025/11/12 19:38:08 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "general.hpp"

void	makeDup2(int const & fd1, int const & fd2, int const & fd3)
{
	if (dup2(fd1, fd2) == -1)
		throw std::runtime_error("Dup2 error");
	close(fd3);
	close(fd1);
}

void	makeDup2(int const & fd1, int const & fd2)
{
	if (dup2(fd1, fd2) == -1)
		throw std::runtime_error("Dup2 error");
	close(fd1);
}

void	dupStandardIO(int *stdFd)
{
	stdFd[FD_STDIN] = dup(STDIN_FILENO);
	stdFd[FD_STDOUT] = dup(STDOUT_FILENO);
	if (stdFd[FD_STDIN] == -1 || stdFd[FD_STDOUT] == -1)
		throw std::runtime_error("Dup error");
}