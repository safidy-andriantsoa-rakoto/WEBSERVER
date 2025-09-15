/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messageUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:13:07 by barandri          #+#    #+#             */
/*   Updated: 2025/09/15 10:31:09 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void send_value(int const & _acceptStatus, std::string & _send_content)
{
    size_t total_sent = 0;
    size_t to_send = _send_content.size();

    std::cout << "\033[048mSIZE=" << to_send << std::endl;
    if (_acceptStatus < 0)
    {
        std::cerr << "INVALID FD TO SEND\n";
        return ;
    }
    while (total_sent < to_send)
    {
        const char *resp =  _send_content.c_str() + total_sent;
        // std::cout << "+++++++++++++++++++++++++++++++++++\n" << resp << "|" << std::endl;
        ssize_t sent = send(_acceptStatus, resp, to_send - total_sent, 0);
        if (sent < 0)
        {
            perror("send failed");
            break;
        }
        else if (sent == 0)
        {
            std::cerr << "Connection closed by peer\n";
            break;
        }
        total_sent += sent; 
    }
    _send_content.clear();
    if (total_sent == to_send)
        std::cout << "Tout le message a été envoyé (" << total_sent << " octets).\n";
    else
        std::cout << "Envoi interrompu après " << total_sent << " octets.\n";
}
