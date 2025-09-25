/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messageUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:13:07 by barandri          #+#    #+#             */
/*   Updated: 2025/09/25 08:13:49 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/*
    Need to throw exception if error on sending? because some connexion stopped from the client
    Other amelioration?
*/
void send_value(int const & _acceptStatus, std::string & _send_content)
{

    size_t to_send = _send_content.size();

    // std::cout << "Size to send = " << _send_content.size() << std::endl;
    if (_acceptStatus < 0)
        throw std::runtime_error("Invalid fd to send");
    const char *resp =  _send_content.c_str();
    ssize_t sent = send(_acceptStatus, resp, to_send, 0);
    if (sent < 0)
    {
        perror("Send failed");
        throw std::runtime_error("Send_value error");
    }
    else if (sent == 0)
        throw std::runtime_error("Connection closed by peer");
    _send_content.clear();
}
