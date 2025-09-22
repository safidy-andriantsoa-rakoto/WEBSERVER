/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckBraceClose.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 10:13:07 by trahanta          #+#    #+#             */
/*   Updated: 2025/09/22 10:05:26 by trahanta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "general.hpp"

bool checkBraceClose(std::vector<std::string> vect)
{
    int count = 0;

    for (size_t i = 0; i < vect.size(); i++)
    {
        for (size_t j = 0; j < vect[i].size(); j++)
            if (vect[i][j] == '{')
            {
                count++;
            }
            else if (vect[i][j] == '}')
            {
                count--;
                if (count < 0)
                {
                    std::cout << "Error : brace not close in config" << std::endl;
                    return false;
                }
            }
    }
    if (count > 0)
    {
        std::cout << "Error : brace not close in config" << std::endl;
        return (false);
    }
    return (true);
}

bool checkBracesAfterNewline(const std::vector<std::string> &file)
{
    for (size_t i = 0; i < file.size(); ++i)
    {
        const std::string &line = file[i];

        for (size_t j = 0; j < line.size(); ++j)
        {
            char c = line[j];

            if (c == '{' || c == '}')
            {
                if (j + 1 < line.size())
                {
                    char nextChar = line[j + 1];

                    if (nextChar != '\n' && nextChar != ' ')
                    {
                        std::cerr << "Error: Character after '"
                                  << c << "' is not allowed '" << std::endl;
                    }
                }
                else
                {
                    continue;
                }
            }
        }
    }

    return true;
}
