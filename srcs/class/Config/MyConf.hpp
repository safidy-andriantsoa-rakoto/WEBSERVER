/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyConf.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:58:12 by barandri          #+#    #+#             */
/*   Updated: 2025/09/11 07:43:20 by trahanta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYCONF_HPP
# define MYCONF_HPP

#include "AConfig.hpp"

class MyConf : public AConfig
{
	public:
		MyConf();
		MyConf(MyConf const & other);
		MyConf & operator=(MyConf const & other);
		~MyConf();

		void 	parseConfig(stringVect file);
		void	parseConfig(std::string const & filePath);
		void	parseConfig(std::string const & servPath, std::string const & locPath);
		bool getSatuts();
		virtual void	displayConfig() const;
	private:
		bool _checkStats;
};

#endif