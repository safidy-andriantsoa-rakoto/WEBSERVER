/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyConf.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsafidy- <rsafidy-@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:58:12 by barandri          #+#    #+#             */
/*   Updated: 2025/10/29 08:20:15 by rsafidy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYCONF_HPP
# define MYCONF_HPP

#include "AConfig.hpp"

class MyConf : public AConfig
{
	private:
		bool _checkStats;
		int  _countLocation;

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

		//barandri_function
		void	myParseLoc(std::string const & filePath);
		void	myParseConfig(std::string const & servPath, std::string const & locPath);


};

#endif