/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyConf.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:58:12 by barandri          #+#    #+#             */
/*   Updated: 2025/10/23 00:57:26 by trahanta         ###   ########.fr       */
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
		int _endLocation;

	public:
		MyConf();
		MyConf(MyConf const & other);
		MyConf & operator=(MyConf const & other);
		~MyConf();

		void	parseConfig(std::string const & filePath);
		void	parseConfig(std::string const & servPath, std::string const & locPath);
		void	getElemetConfig(const size_t &start, const size_t &end, const stringVect &file);

		void 	parseConfig(stringVect file);

		bool 	getStatus();
		size_t findNotDigitError();

		virtual void	displayConfig() const;

		vectLocation createVectLocation(size_t countLocation);
};

#endif