/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:54:28 by rsafidy-          #+#    #+#             */
/*   Updated: 2025/10/24 13:17:14 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "general.hpp"
#include "../Response/ErrorExcept.hpp"

static MapMime _mime;

class Mime
{
	private:
		Mime();
		Mime(const Mime& mime);
		Mime& operator=(const Mime& mime);

		static void	get_file_content_mime(std::ifstream &fd);
		static void	trim_space(std::string &data);

	public:
		static MapMime _mime;

		Mime(stringVect &path);
		MapMime get_mime() const;
		~Mime();
		void check_content_type();
		
		static void			initMapMime(std::string const & path);
		static MapMime &	getMapMime();
		static std::string	getFileType(std::string const & file);
};
