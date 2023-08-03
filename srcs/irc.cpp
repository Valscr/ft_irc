/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyacini <kyacini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 02:25:47 by valentin          #+#    #+#             */
/*   Updated: 2023/07/31 20:31:55 by kyacini          ###   ########.fr       */
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
