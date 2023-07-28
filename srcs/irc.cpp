/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 02:25:47 by valentin          #+#    #+#             */
/*   Updated: 2023/07/28 00:51:26 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"

std::string find_next_word(int i, std::string str)
{
    int j = i;
    while (str[i] != '\r' && str[i] != '\0' && str[i] != ' ' && str[i] != '\n')
        i++;
    return (str.substr(j, i - j));
}

std::string find_previous_word(int i, std::string str)
{
    int j = i - 1;
    while (j >= 0 && str[j] != '\r' && str[j] != ' ' && str[j] != '\n')
        j--;
    return str.substr(j + 1, (i - 1) - j);
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

void send_function(Server &server, std::vector<pollfd> fds)
{
    (void)fds;
    for (std::map<int, std::string>::const_iterator it = server.get_send().begin(); it != server.get_send().end(); ++it)
    {
        if (!it->second.empty())
        {
            {
                const char* data = it->second.c_str();
                std::cout << ANSI_GREEN << it->first << " > " << data << ANSI_RESET << std::endl;
                send(it->first, data, std::strlen(data), 0);
            }
            server.erase_send(it->first);
        }
    }
}
