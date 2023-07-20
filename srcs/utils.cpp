/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smecili <smecili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 11:24:30 by valentin          #+#    #+#             */
/*   Updated: 2023/07/20 17:46:29 by smecili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"

int is_port(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (!std::isdigit(str[i]))
        {
            std::cout << "Error: port not allowed" << std::endl;
            return (0);
        }
    }
    if (std::atoi(str) < 0 || std::atoi(str) > 65535)
    {
        std::cout << "Error: port not allowed\n" << std::endl;
        return (0);
    }
    return (1);
}