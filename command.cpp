/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 17:55:43 by valentin          #+#    #+#             */
/*   Updated: 2023/07/09 22:55:34 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void join_funct(std::string buffer, Server &server, int fd)
{
    if (server.find_channel(find_next_word(6, buffer)))
    {
        server.getChannel(find_next_word(6, buffer)).addWhiteList(fd);
        send_whitelist(server, fd, find_next_word(6, buffer), (":" + server.getUser(fd).returnNickname() + " " + buffer).c_str());
        server.get_send_fd(fd).append((":" + server.getUser(fd).returnNickname() + " " + buffer).c_str());
        server.get_send_fd(fd).append((":" + std::string(SERVER_NAME) + " 332 " + server.getUser(fd).returnNickname() + " " + find_next_word(6, buffer) + " :Channel topic here\r\n").c_str());
        server.get_send_fd(fd).append((":" + std::string(SERVER_NAME) + " 353 " + server.getUser(fd).returnNickname() +  " = #" + find_next_word(6, buffer) + " :" + msg_353(server, find_next_word(6, buffer)) + "\r\n").c_str());
        server.get_send_fd(fd).append((":" + std::string(SERVER_NAME) + " 366 " + server.getUser(fd).returnNickname() + " #" + find_next_word(6, buffer) + " :End of NAMES list\r\n").c_str());
    }
    else
    {
        server.createChannel(find_next_word(6, buffer), fd);
        server.get_send_fd(fd).append((":" + server.getUser(fd).returnNickname() + " " + buffer).c_str());
        server.get_send_fd(fd).append((":" + std::string(SERVER_NAME) + " 332 " + server.getUser(fd).returnNickname() + " " + find_next_word(6, buffer) + " :Channel topic here\r\n").c_str());
    }
}

void privmsg_funct(std::string buffer, Server &server, int fd)
{
    if (buffer.find("PRIVMSG #") != std::string::npos)
    {
        if (server.find_channel(find_next_word(9, buffer)))
            send_whitelist(server, fd, find_next_word(9, buffer), (":" + server.getUser(fd).returnNickname() + " " + buffer).c_str());
        else
            server.get_send_fd(fd).append((":" + std::string(SERVER_NAME) + " 401 " + server.getUser(fd).returnNickname() + " " + find_next_word(8, buffer) + " :No such channel\r\n").c_str());
    }
    else
    {
        if (server.UserExist(find_next_word(8, buffer)))
            server.get_send_fd(server.getUserwithNickname(find_next_word(8, buffer)).returnFd()).append((":" + server.getUser(fd).returnNickname() + " " + buffer).c_str());
        else
        {
            server.get_send_fd(fd).append((":" + std::string(SERVER_NAME) + " 301 " + server.getUser(fd).returnNickname() + " " + find_next_word(8, buffer) + " :This user is disconnected.\r\n").c_str());
        }
    }
}