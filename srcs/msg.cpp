/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyacini <kyacini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 14:23:00 by valentin          #+#    #+#             */
/*   Updated: 2023/07/31 20:32:44 by kyacini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"

std::string msg_001(std::string user)
{
    return (( ":" + std::string(SERVER_NAME) + " 001 " + user + " :Welcome on " + std::string(SERVER_NAME) + " " + user + "\n").c_str());
}

void msg_421()
{
    std::cout << ":" + std::string(SERVER_NAME) + " 421 client :Unknown command\n" << std::endl;
}
void msg_464()
{
    std::cout << ":" + std::string(SERVER_NAME) + " 464 client :Password required\n" << std::endl;
}

void msg_462()
{
    std::cout << ":" << std::string(SERVER_NAME) << "462 :Unauthorized command (already registered)\r\n" << std::endl;
}

void msg_431()
{
    std::cout << ":" << std::string(SERVER_NAME) << "431 :Nickname is already in use\r\n" << std::endl;
}

void msg_432()
{
    std::cout << ":" << std::string(SERVER_NAME) << "432 :Erroneus nickname\r\n" << std::endl;
}

void msg_433()
{
    std::cout << ":" << std::string(SERVER_NAME) << "433 :Nickname already in use\r\n" << std::endl;
}