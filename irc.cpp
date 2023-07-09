/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 02:25:47 by valentin          #+#    #+#             */
/*   Updated: 2023/07/10 00:08:44 by valentin         ###   ########.fr       */
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
        {
            server.get_send_fd(*it).append(buffer);
        }
    }
    for (std::vector<int>::iterator it = server.getChannel(channel).getWhiteList().begin(); it != server.getChannel(channel).getWhiteList().end(); ++it)
    {
        if (fd != *it)
        {
            server.get_send_fd(*it).append(buffer);
        }
    }
}

void parse_buffer(std::string buffer, Server &server, int fd)
{
    if (buffer.find("PING") != std::string::npos)
        server.get_send_fd(fd).append(("PONG " + server.get_name() + "\r\n").c_str());
    if (buffer.find("JOIN #") != std::string::npos)
        join_funct(buffer, server, fd);
    if (buffer.find("PRIVMSG") != std::string::npos)
        privmsg_funct(buffer, server, fd);
}

void send_function(Server &server, std::vector<pollfd> fds)
{
    for (std::map<int, std::string>::const_iterator it = server.get_send().begin(); it != server.get_send().end(); ++it)
    {
        if (!it->second.empty())
        {
            {
                const char* data = it->second.c_str();
                std::cout << ANSI_GREEN << MAX_CLIENTS - it->first + 1 << " > " << data << ANSI_RESET << std::endl;
                send(fds[it->first].fd, data, std::strlen(data), 0);
            }
            server.erase_send(it->first);
        }
    }
}
