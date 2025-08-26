/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:54:28 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/08/12 11:13:29 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "general.hpp"

class Mime
{
    private:
        MapMime _mime;
        stringVect path;
        Mime();
        Mime(const Mime& mime);
        Mime& operator=(const Mime& mime);

    public:
        Mime(stringVect &path);
        void get_file_content_mime(std::ifstream &fd);
        void trim_space(std::string &data);
        ~Mime();
};
