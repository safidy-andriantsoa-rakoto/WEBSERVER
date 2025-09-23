/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messageUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:13:07 by barandri          #+#    #+#             */
/*   Updated: 2025/09/23 09:04:14 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/*
    Need to throw exception if error on sending? because some connexion stopped from the client
    Other amelioration?
*/
void send_value(int const & _acceptStatus, std::string & _send_content)
{

    std::cout << "SIZE = " << _send_content.size() << std::endl;
    if (_acceptStatus < 0)
    {
        std::cerr << "INVALID FD TO SEND\n";
        return ;
    }
    ssize_t sent = send(_acceptStatus, _send_content.c_str(), _send_content.size(), 0);
    if (sent < 0)
        perror("Send failed");
    else if (sent == 0)
        std::cerr << "Connection closed by peer\n";
    if (sent <= 0)
        throw std::runtime_error("Sending error");
    _send_content.clear();
}
