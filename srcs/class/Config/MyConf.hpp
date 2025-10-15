/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyConf.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barandri <barandri@student.42antananarivo. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:58:12 by barandri          #+#    #+#             */
/*   Updated: 2025/10/15 10:35:02 by barandri         ###   ########.fr       */
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

		bool	getStatus();
		void	getElemetConfig(const size_t &start, const size_t &end, const stringVect &file);
		void 	parseConfig(stringVect file);

		size_t			findNotDigitError();
		vectLocation	createVectLocation(size_t countLocation);

		//barandri_function
		void	locationParsing(std::string const & filePath);
		void	myParseConfig(std::string const & servPath, std::string const & locPath);

		//For testing
		virtual void	displayConfig() const;

};

#endif