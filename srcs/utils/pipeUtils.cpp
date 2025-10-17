/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 14:23:44 by barandri          #+#    #+#             */
/*   Updated: 2025/10/17 14:42:55 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "general.hpp"

void	makeDup2(int const & fd1, int const & fd2, int const & fd3)
{
	dup2(fd1, fd2);
	close(fd3);
	close(fd1);
}

void	makeDup2(int const & fd1, int const & fd2)
{
	dup2(fd1, fd2);
	close(fd1);
}

void	dupStandardIO(int *stdFd, int const & n)
{
	if (n == 2)
	{
		stdFd[FD_STDOUT] = dup(STDOUT_FILENO);
		stdFd[FD_STDERR] = dup(STDERR_FILENO);
	}
	if (n == 3)
		stdFd[FD_STDIN] = dup(STDIN_FILENO);
}