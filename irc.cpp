/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 02:25:47 by valentin          #+#    #+#             */
/*   Updated: 2023/07/08 02:27:12 by valentin         ###   ########.fr       */
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

std::string parse_buffer(std::string buffer)
{
    if (buffer.find("PING") != std::string::npos)
        return ("PONG server-irc\n");
    return ("");
}

void send_function(std::vector<std::string> &send_client, int i, std::vector<pollfd> fds)
{
    std::vector<std::string>::const_iterator it;
    for (it = send_client.begin(); it != send_client.end(); ++it)
    {
        const std::string& str = *it;
        const char* data = str.c_str(); 
        send(fds[i].fd, data, std::strlen(data), 0);
    }
    send_client.clear();
}
