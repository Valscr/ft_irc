/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smecili <smecili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 14:23:00 by valentin          #+#    #+#             */
/*   Updated: 2023/07/20 17:45:35 by smecili          ###   ########.fr       */
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

std::string msg_353(Server &server, std::string channel)
{
    std::string str = "";
    for (std::vector<int>::iterator it = server.getChannel(channel).getOperators().begin(); it != server.getChannel(channel).getOperators().end(); ++it)
    {
        str.append("@");
        str.append(server.getUser(*it).returnNickname());
        str.append(" ");
    }
    for (std::vector<int>::iterator it = server.getChannel(channel).getWhiteList().begin(); it != server.getChannel(channel).getWhiteList().end(); ++it)
    {
        str.append(server.getUser(*it).returnNickname());
        str.append(" ");
    }
    return (str);
}