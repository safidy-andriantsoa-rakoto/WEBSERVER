/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messageUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:13:07 by barandri          #+#    #+#             */
/*   Updated: 2025/08/01 09:05:29 by barandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void send_value(int &_acceptStatus, std::string &_send_content)
{
    size_t tb = 0;
    while (tb < _send_content.size())
    {
        size_t _send_request = send(_acceptStatus, _send_content.c_str(), _send_content.size(), 0);
        if (!_send_request)
            std::cout << "Error\nSend" << std::endl;
        tb += _send_request;
    }
}