/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyConf.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:58:12 by barandri          #+#    #+#             */
/*   Updated: 2025/11/05 11:14:26 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYCONF_HPP
# define MYCONF_HPP

#include "AConfig.hpp"

class MyConf : public AConfig
{
	private:
		bool	_checkStats;
		int		_countLocation;
		int		_endLocation;

	public:
		MyConf();
		MyConf(MyConf const & other);
		MyConf & operator=(MyConf const & other);
		~MyConf();

		bool			getStatus();
		void			getElementConfig(const size_t &start, const size_t &end, const stringVect &file);
		void 			parseConfig(stringVect file);
		size_t			findNotDigitError();
		vectLocation	createVectLocation(size_t countLocation);
		void			setHostName(std::string host);
		//For testing
		virtual void	displayConfig() const;

		/* [Test only] */
		void	myParseLoc(std::string const & filePath);
		void	myParseConfig(std::string const & servPath, std::string const & locPath);
		void	init_port_child(int port);
		void	delete_port_child_in_parent();
};

#endif