/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messageUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:13:07 by barandri          #+#    #+#             */
/*   Updated: 2025/08/29 14:17:41 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void send_value(int const & _acceptStatus, std::string & _send_content)
{
    size_t total_sent = 0;
    size_t to_send = _send_content.size();

    while (total_sent < to_send)
    {
        ssize_t sent = send(_acceptStatus,_send_content.c_str() + total_sent, to_send - total_sent, 0);
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
    if (total_sent == to_send)
        std::cout << "Tout le message a été envoyé (" << total_sent << " octets).\n";
    else
        std::cout << "Envoi interrompu après " << total_sent << " octets.\n";
}
