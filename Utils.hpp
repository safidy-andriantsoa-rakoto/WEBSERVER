/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:46:55 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/07/28 09:48:58 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <iterator>
#include <iostream>

#define BADREQUEST "HTTP/1.1 400 Bad Request\n\r\n<html><head></head> <body><h1 background=\"#ff0000\" >ERROR 400 BAD REQUEST.</h1></body></html>"

std::string response_generator(std::string response,std::string content, std::string type, size_t size);
std::string get_index_content(std::string _path);
void        send_value(int &_acceptStatus, std::string &_send_content);
void        split_line(std::vector<std::string> &_data, std::string line);
void        get_request(std::string _client_get, int &_acceptStatus, std::string path);