/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 14:23:00 by valentin          #+#    #+#             */
/*   Updated: 2023/07/08 14:46:37 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string msg_001(std::string user)
{
    return (( ":" + std::string(SERVER_NAME) + " 001 " + user + " :Welcome on " + std::string(SERVER_NAME) + " " + user + "\n").c_str());
}

std::string msg_464()
{
    return ((":" + std::string(SERVER_NAME) + " 464 client :Password required\n").c_str());
}
