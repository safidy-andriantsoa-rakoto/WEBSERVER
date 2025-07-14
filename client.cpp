/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 08:28:46 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/07/10 09:20:58 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

static std::string get_index_file(std::string path)
{
}

int main(int ac, char **argv)
{
    // char resp[1024] = "";
    // char *sm = argv[1];
    // char sms[1024] = sm;
    if(ac ==1)
        return (0);
    int _socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFd == -1)
        std::cout << "Error\ncreate an endpoint for communication" << std::endl;

    struct sockaddr_in _sockaddrIn;
    _sockaddrIn.sin_family = AF_INET;
    _sockaddrIn.sin_port = htons(2003);
    _sockaddrIn.sin_addr.s_addr = INADDR_ANY;

    int size = sizeof(_sockaddrIn);
    int _connectStatus = connect(_socketFd, (sockaddr *)&_sockaddrIn, size);
    if (_connectStatus == -1)
        std::cout << "Error\n Connexion lost" << std::endl;
    int _sendStatus = send(_socketFd, argv[1], sizeof(argv[1]), 0);
    if (_sendStatus == -1)
        std::cout << "Error\nFailed to sent message!" << std::endl;
    else
        std::cout << "Success!" << std::endl;
    close(_socketFd);
    close(_connectStatus);
    return 0;
}
