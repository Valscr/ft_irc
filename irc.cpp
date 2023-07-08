/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 02:25:47 by valentin          #+#    #+#             */
/*   Updated: 2023/07/08 20:48:57 by valentin         ###   ########.fr       */
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

void parse_buffer(std::string buffer, Server &server, int fd)
{
    if (buffer.find("PING") != std::string::npos)
        server.get_send().push_back(("PONG " + server.get_name() + "\r\n").c_str());
    if (buffer.find("JOIN #") != std::string::npos)
    {
        server.createChannel(find_next_word(6, buffer), fd);
        server.get_send().push_back((":" + server.getUser(fd).returnNickname() + " " + buffer).c_str());
        server.get_send().push_back((":" + std::string(SERVER_NAME) + " 332 " + server.getUser(fd).returnNickname() + " " + find_next_word(6, buffer) + " :Channel topic here\r\n").c_str());
    }
}

void send_function(std::vector<std::string> &send_client, int i, std::vector<pollfd> fds)
{
    std::vector<std::string>::const_iterator it;
    for (it = send_client.begin(); it != send_client.end(); ++it)
    {
        const std::string& str = *it;
        if (!str.empty())
        {
            const char* data = str.c_str();
            std::cout << "> " << data << std::endl;
            send(fds[i].fd, data, std::strlen(data), 0);
        }
    }
    send_client.clear();
}
