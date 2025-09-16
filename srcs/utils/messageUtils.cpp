/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messageUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:13:07 by barandri          #+#    #+#             */
/*   Updated: 2025/09/16 10:46:07 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void send_value(int const & _acceptStatus, std::string & _send_content)
{

    std::cout << "\033[048mSIZE=" << _send_content.size() << std::endl;
    if (_acceptStatus < 0)
    {
        std::cerr << "INVALID FD TO SEND\n";
        return ;
    }
    // const char *resp =  _send_content.c_str();
    // std::cout << "+++++++++++++++++++++++++++++++++++\n" << resp << "|" << std::endl;
    ssize_t sent = send(_acceptStatus, _send_content.c_str(), _send_content.size(), 0);
    if (sent < 0)
    {
        perror("send failed");
    }
    else if (sent == 0)
    {
        std::cerr << "Connection closed by peer\n";
    }
    _send_content.clear();
}
