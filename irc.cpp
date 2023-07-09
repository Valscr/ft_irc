/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 02:25:47 by valentin          #+#    #+#             */
/*   Updated: 2023/07/09 14:07:20 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string find_next_word(int i, std::string str)
{
    int j = i;
    while (str[i] != '\r' && str[i] != '\0' && str[i] != ' ' && str[i] != '\n')
        i++;
    return (str.substr(j, i - j));
}

void send_whitelist(Server &server, int fd, std::string channel, std::string buffer)
{
    for (std::vector<int>::iterator it = server.getChannel(channel).getOperators().begin(); it != server.getChannel(channel).getOperators().end(); ++it)
    {
        if (fd != *it)
            server.get_send_fd(*it).append(buffer);
    }
    for (std::vector<int>::iterator it = server.getChannel(channel).getWhiteList().begin(); it != server.getChannel(channel).getWhiteList().end(); ++it)
    {
        if (fd != *it)
            server.get_send_fd(*it).append(buffer);
    }
}

void parse_buffer(std::string buffer, Server &server, int fd)
{
    if (buffer.find("PING") != std::string::npos)
        server.get_send_fd(fd).append(("PONG " + server.get_name() + "\r\n").c_str());
    if (buffer.find("JOIN #") != std::string::npos)
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
    if (buffer.find("PRIVMSG") != std::string::npos)
    {
        if (buffer.find("PRIVMSG #") != std::string::npos)
        {
            if (server.find_channel(find_next_word(9, buffer)))
            {
                send_whitelist(server, fd, find_next_word(9, buffer), (":" + server.getUser(fd).returnNickname() + " " + buffer).c_str());
            }
            else
                server.get_send_fd(fd).append((":" + std::string(SERVER_NAME) + " 401 " + server.getUser(fd).returnNickname() + " " + find_next_word(8, buffer) + " :No such channel\r\n").c_str());

        }
        else
        {
            if (server.UserExist(find_next_word(8, buffer)))
            {
                server.get_send_fd(server.getUserwithNickname(find_next_word(8, buffer)).returnFd()).append((":" + server.getUser(fd).returnNickname() + " " + buffer).c_str());
                server.get_send_fd(fd).append((":" + std::string(SERVER_NAME) + " 301 " + server.getUser(fd).returnNickname() + " " + find_next_word(8, buffer) + " :Message sent successfully\r\n").c_str());
            }
            else
                server.get_send_fd(fd).append((":" + std::string(SERVER_NAME) + " 401 " + server.getUser(fd).returnNickname() + " " + find_next_word(8, buffer) + " :No such User\r\n").c_str());
        }
    }
}

void send_function(Server &server, std::vector<pollfd> fds)
{
    for (std::map<int, std::string>::const_iterator it = server.get_send().begin(); it != server.get_send().end(); ++it)
    {
        if (!it->second.empty())
        {
            {
                const char* data = it->second.c_str();
                std::cout << "> " << data << std::endl;
                send(fds[it->first].fd, data, std::strlen(data), 0);
            }
            server.erase_send(it->first);
        }
    }
}
